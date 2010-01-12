/*************************************************************
 * aci.cpp
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#include <stdio.h>
#include <assert.h>
#include <libimli.h>
#include <QFile>
#include <QDebug>

#include "TextEdit.h"

int LoadISCII(TextEdit* editor, QString filename, imli_script_t default_script)
{
    TextStorage* text = editor->GetTextStorage();
    assert(text);

    text->MoveTo(0);

    imli_context_t ctx = text->GetContext();
    if (!imli_is_context_valid(ctx)) return -1;

    if (init_iscii_tables(ctx) != IMLI_STATUS_OK) return -1;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) return -1;

    // slurp file into a buffer
    QByteArray bytes = file.readAll();
    file.close();

    const byte_t* pbuf = (const byte_t*)bytes.constData();    
    std::vector<syl_t> syl_buf;
    int nbytes = bytes.size();
    syl_buf.reserve(nbytes);

    syl_t prev_syl = SYL_INVALID;
    imli_script_t prev_script = default_script;
    imli_script_t cur_script = SCRIPT_ASCII;
    for(int i = 0; i < nbytes; i++)
    {
        byte_t b = pbuf[i];
       
        // to fix errors in ISCII files
        if (b >= 0xA0 && b != 0xEF && cur_script == SCRIPT_ASCII)
        {
            qDebug() << "=======> Applying FIX: cur_script = " << cur_script << " prev_script = " << prev_script << " byte = " << b;
            if (!syl_buf.empty())
            {
                text->Insert(cur_script, &syl_buf[0], syl_buf.size());
                syl_buf.clear();
            }
            cur_script = prev_script;
            imli_select_script(ctx, cur_script);
            prev_syl = SYL_INVALID;
        }
        //fprintf(stderr, "\n\n #############  BYTE = %02x\tCUR_SCRIPT = %d\n\n", b, cur_script);
        // common case
        if (isspace(b) || b == ';' || b == '!' ||
            b == '\''  || b == '/' || b == ',' ||
            b == '?'   || b == '.' || b == '-' )
            //b == '(' || b == ')')
        {
            //fprintf(stderr, "\n\n************  whitespace! byte = %d, script = %d ************\n\n", b, cur_script);
            // skip cr
            if (b != 13)
                syl_buf.push_back(ASCII_START + b);
            continue;
        }
 
        /* Marathi specific */
        if (i + 1 < nbytes && cur_script == SCRIPT_MARATHI &&
            b == 0xAE && pbuf[i+1] == 0xE9 )
        {
            //fprintf(stderr, "MARATHI recognized!!!\n\n");
            syl_buf.push_back(SYLLABLE(63, 0, 15));
            i++;
            continue;
        }

        // specials
        if ((cur_script != SCRIPT_ASCII ) &&
            (/*b == 28 || b == 29 || */
                b == 0x2C || b == 0x2D ||
                b == 0xA1 || b == 0xAE || b == 0xEA))
        {
            imli_compose_flag_t flags;
            imli_char_type_t type = TYPE_INVALID;
            byte_t imli_value = 0;
            iscii_to_imli(ctx, b, &imli_value, &type, &flags);
            if (type != TYPE_INVALID)
                syl_buf.push_back(SYLLABLE(63, 0, imli_value));
            continue;
        }

        /* FIXME: numbers in english is causing a language switch. As
         * a workaround, switching all english numbers to their local
         * language equivalent.
         */

        if (b >= '0' && b <= '9' && cur_script != SCRIPT_ASCII)
        {
            syl_buf.push_back(SYLLABLE(63, 3, b));
            continue;
        }

        // ascii
        if (b < 127)
        {
            if (cur_script != SCRIPT_ASCII)
            {
                //fprintf(stderr, ">>>>>>>>>>>> ASCII switch: i = %d, cur_script = %d, nsyls = %d <<<<<<<<<<<<\n\n", i, cur_script, syl_buf.size());
                /* insert text converted so far and switch to ascii script */
                if (!syl_buf.empty())
                {
                    text->Insert(cur_script, &syl_buf[0], syl_buf.size());
                    syl_buf.clear();
                }
                prev_syl = SYL_INVALID;
                prev_script = cur_script;
                cur_script = SCRIPT_ASCII;
                imli_select_script(ctx, cur_script);
            }
            syl_buf.push_back(ASCII_START + b);
            continue;
        }

        // numbers in local language
        if (b >= 0xF1 && b <= 0xFA && cur_script != SCRIPT_ASCII)
        {
            byte_t num = b - 0xF1;
            syl_buf.push_back(SYLLABLE(63, 3, num));
            continue;
        }

        // language switch
        if (b == 0xEF)
        {
            if (i + 1 < nbytes)
            {
                i++;
                imli_script_t script = script_from_iscii_code(pbuf[i]);
                //fprintf(stderr, "Langswitch: index = %d, byte = %x, script = %d\n\n", i, pbuf[i+1], script);
                if (script != SCRIPT_UNSUPPORTED && script != cur_script && script != SCRIPT_SANSKRIT)
                {
                    //fprintf(stderr, ">>>>>>>>>>>> lang switch: i = %d, cur_script = %d, code = %d, nsyls = %d <<<<<<<<<<<<\n\n", i, cur_script, script, syl_buf.size());
                    /* lang switch - insert the text converted so far */
                    if (syl_buf.size())
                    {
                        text->Insert(cur_script, &syl_buf[0], syl_buf.size());
                        syl_buf.clear();
                    }

                    imli_select_script(ctx, script);
                    prev_script = cur_script;
                    cur_script = script;
                    prev_syl = SYL_INVALID;
                }
            }
            continue;
        }
        else
        {
            imli_compose_flag_t flags;
            imli_char_type_t type = TYPE_INVALID;
            byte_t imli_value = 0;

            if (i + 1 < nbytes && pbuf[i+1] == 0xE9)
            {
                type = TYPE_CONSONANT;
                flags = FLAG_NONE;
                i++;
                if (b == 0xbf)
                    imli_value = 43; // ddra
                else if (b == 0xc0)
                    imli_value = 44; // ddhra
                else
                {
                    i--;
                    type = TYPE_INVALID;
                }
            }
            /* special cases for promoted consonants */
            if (i + 2 < nbytes)
            {
                type = TYPE_CONSONANT;
                i += 2;
                if (b == 0xb3 && pbuf[i - 1] == 0xe8 && pbuf[i] == 0xd6)
                    imli_value = 45;  // ksha
                else if (b == 0xba && pbuf[i - 1] == 0xe8 && pbuf[i] == 0xbc)
                    imli_value = 46; // jnya
                else
                {
                    i -= 2;
                    type = TYPE_INVALID;
                }
            }

            if (type != TYPE_INVALID)
            {
                int del_prev_syl = 0;
                syl_t new_syl = imli_syl_compose(ctx, imli_value, type, cur_script, prev_syl, flags, &del_prev_syl);
                if (new_syl != SYL_INVALID)
                {
                    //fprintf(stderr, "====== alive! i = %d, value = %d ======\n\n", i, imli_value);
                    if (del_prev_syl)
                        syl_buf[syl_buf.size()-1] = new_syl;
                    else
                        syl_buf.push_back(new_syl);
                    prev_syl = new_syl;
                }
                continue;
            }

            /* handle script specific issues such as the
             *  North-South vowel differences.
             */
            if (int(cur_script) >= 2 && int(cur_script) <= 5)
            {
                iscii_to_imli(ctx, b, &imli_value, &type, &flags);
                // if (b == 0xAB)
//                 {
//                     /* southern scripts - short a */
//                     imli_value = 7;
//                     type = TYPE_VOWEL;
//                     flags = FLAG_STANDALONE_VOWEL;
//                 }
//                 else if (b == 0xE0)
//                 {
//                     /* southern scripts - short a matra */
//                     imli_value = 7;
//                     type = TYPE_VOWEL;
//                     flags = FLAG_NONE;
//                 }
//                 else if (b == 0xAF)
//                 {
//                     /* southern scripts - short o */
//                     imli_value = 10;
//                     type = TYPE_VOWEL;
//                     flags = FLAG_STANDALONE_VOWEL;
//                 }
//                 else if (b == 0xE4)
//                 {
//                     imli_value = 10;
//                     type = TYPE_VOWEL;
//                     flags = FLAG_NONE;
//                 }
//                 else
//                 {
//                     iscii_to_imli(ctx, b, &imli_value, &type, &flags);
//                 }
            }
            else if (b == 0xE9)
            {
                type = TYPE_CONSONANT;
                imli_value = 54;
                flags = FLAG_NONE;
            }
            else
            {
                iscii_to_imli(ctx, b, &imli_value, &type, &flags);
            }
            //fprintf(stderr, "b = %x\timli_value = %d, type = %d, flags = %d\n", b, imli_value, type, flags);
            int del_prev_syl = 0;
            syl_t new_syl = imli_syl_compose(ctx, imli_value, type, cur_script, prev_syl, flags, &del_prev_syl);
            if (new_syl != SYL_INVALID)
            {
                if (del_prev_syl)
                    syl_buf[syl_buf.size()-1] = new_syl;
                else
                    syl_buf.push_back(new_syl);
                prev_syl = new_syl;
            }
        }
    }
    if (syl_buf.size())
        text->Insert(cur_script, &syl_buf[0], syl_buf.size());

    return 0;
}

int SaveISCII(TextEdit* editor, QString filename)
{
    TextStorage* text = editor->GetTextStorage();
    if (text->Size() <= 0)
        return -1;
    
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return -1;

    TextStorage::Selection sel = text->GetText();
    
    while (sel.IsValid())
    {
        SylString s = sel.GetData();
        size_t insize = s.size();

        /* 16 bytes per syllable - should be checked when 4th level
         * conjuncts are added.
         */
        size_t outsize = insize*16;
        byte_t* outbuf = new byte_t[outsize];
        imli_script_t script = sel.GetScript();
        int i = 0;
        
        imli_to_iscii(text->GetContext(), script, s.data(), insize, outbuf, &outsize);

        qDebug() << "SaveISCII(): writing " << outsize << " bytes...";

        file.write((const char*)outbuf, outsize + i);
        delete[] outbuf;
        
        sel.NextRun();
    }
    file.close();
    return 0;
}
