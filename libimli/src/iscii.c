/*************************************************************
 * iscii.c
 *
 * This file is part of libimli
 *
 * Licensed under the GNU Library General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#define __IMLI_IMPL__

#include "libimli.h"
#include "imli_private.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const byte_t halant    = 0xE8;
const byte_t nukhta    = 0xE9;
const byte_t nukhta_code = 54;

const byte_t nex       = 21;
const byte_t nex_iscii = 0xC6;
const byte_t rex       = 31;
const byte_t rxl       = 32;
const byte_t rxxl       = 33;
const byte_t rex_iscii = 0xCF;

const byte_t ksha  = 45;
const byte_t jnya  = 46;
const byte_t ddra  = 43;
const byte_t ddrha = 47;

/* ISCII language codes. The actual switch code is the byte_t 0xef. */
static const byte_t lang_switch_codes[] = {
    0x52, // sanskrit
    0x42, // hindi
    0x44, // tamil
    0x45, // telugu
    0x48, // kannada
    0x49, // malayalam
    0x47, // oriya
    0x43, // bangla
    0x4a, // gujarati
    0x4b, // punjabi 
    0x46, // assamese
    0x20, // unassigned - so using a space
    0x60, // marathi (same as hindi)   
};

#include "iscii_data.h"

static int setup_iscii_imli_table (iscii_t iscii)
{
    if (!iscii) return -1;

    int i = 0;

    /* 0-127 is ASCII */

    for (i = 0; i < 128; i++)
    {
        iscii->iscii_imli_map[i].type = TYPE_ASCII;
        iscii->iscii_imli_map[i].value = i;
        iscii->iscii_imli_map[i].flags = FLAG_NONE;
    }

    /* standalone vowels */
    for (i = 0; i < 16; i++)
    {
        iscii->iscii_imli_map[iscii->vowels[i]].type  = TYPE_VOWEL;
        iscii->iscii_imli_map[iscii->vowels[i]].value = i;
        iscii->iscii_imli_map[iscii->vowels[i]].flags = FLAG_STANDALONE_VOWEL;
    }

    /* consonants */
    for (i = 0; i < iscii->nconsonants; i++)
    {
        iscii->iscii_imli_map[iscii->consonants[i]].type  = TYPE_CONSONANT;
        iscii->iscii_imli_map[iscii->consonants[i]].value = i;
        iscii->iscii_imli_map[iscii->consonants[i]].flags = FLAG_NONE;
    }

    /* vowel matras */
    for (i = 0; i < 16; i++)
    {
        iscii->iscii_imli_map[iscii->vowels_m_form[i]].type  = TYPE_VOWEL;
        iscii->iscii_imli_map[iscii->vowels_m_form[i]].value = i;
        iscii->iscii_imli_map[iscii->vowels_m_form[i]].flags = FLAG_NONE;
    }

    /* specials */
    for (i = 0; i < iscii->nspecials; i++)
    {
        iscii->iscii_imli_map[iscii->specials[i]].type  = TYPE_SPECIAL;
        iscii->iscii_imli_map[iscii->specials[i]].value = i;
        iscii->iscii_imli_map[iscii->specials[i]].flags = FLAG_NONE;
    }

    return 0;
}

/*
  \brief initialize the ISCII tables.
  \param ctx - IMLI context
  \return IMLI_STATUS_OK on success and other error codes on errors.
*/

EXPORT_C imli_status_t init_iscii_tables (imli_context_t ctx)
{
    if (!imli_is_context_valid(ctx)) return IMLI_STATUS_INVALID_CONTEXT;

    if (ctx->iscii)
    {
        ctx->iscii->cur_lang = 0xFF;
        return IMLI_STATUS_OK;
    }

    iscii_t iscii = NULL;
    iscii = (iscii_t)malloc(sizeof(struct iscii_context));    
    if (!iscii) return IMLI_STATUS_NO_MEMORY;
    memset(iscii, 0, sizeof(struct iscii_context));
    
    iscii->nvowels = (sizeof(imli_to_iscii_vow)/sizeof(imli_to_iscii_vow[0]));
    iscii->vowels  = imli_to_iscii_vow;

    iscii->vowels_m_form  = imli_to_iscii_vow_matras;

    iscii->nconsonants = (sizeof(imli_to_iscii_con)/sizeof(imli_to_iscii_con[0]));
    iscii->consonants  = imli_to_iscii_con;

    iscii->nspecials   = (sizeof(imli_to_iscii_spl)/sizeof(imli_to_iscii_spl[0]));
    iscii->specials    = imli_to_iscii_spl;

    if (setup_iscii_imli_table(iscii) != 0)
    {
        free(iscii);
        ctx->iscii = NULL;
        return IMLI_STATUS_ISCII_DATA_NOT_FOUND;
    }
    ctx->iscii = iscii;
    return IMLI_STATUS_OK;
}

/*!
  \brief return the ISCII equivalent for the supplied IMLI script
  \param script IMLI script code
  \return the ISCII script code
*/

EXPORT_C byte_t iscii_code_from_script (imli_script_t script)
{
    return lang_switch_codes[script];
}

EXPORT_C imli_script_t script_from_iscii_code (byte_t code)
{
    unsigned int i = 0;
    for (i = 0; i < sizeof(lang_switch_codes)/sizeof(lang_switch_codes[0]); i++)
    {
        if (code == lang_switch_codes[i])
            return (imli_script_t)i;
    }
    return SCRIPT_UNSUPPORTED;
}

/*
  \brief ISCII to IMLI conversion. It is required that \fn init_iscii_tables be called \e before calling this function.
  \param[in] - the iscii value
  \param[out] - the equivalent IMLI character code
  \param[out] - the type of the IMLI character type
  \param[out[ - addition information to assist syllable composition
  \returns IMLI_STATUS_OK on success and IMLI_STATUS_BAD_INPUT on error(s).
*/
EXPORT_C imli_status_t iscii_to_imli (imli_context_t ctx, 
                             byte_t iscii, 
                             byte_t *imli, 
                             imli_char_type_t *type,
                             imli_compose_flag_t *flags)
{
    *imli  = ctx->iscii->iscii_imli_map[iscii].value;
    *type  = ctx->iscii->iscii_imli_map[iscii].type;
    *flags = ctx->iscii->iscii_imli_map[iscii].flags;
    return IMLI_STATUS_OK;
}

EXPORT_C imli_status_t imli_to_iscii (imli_context_t ctx, imli_script_t script,
                             const syl_t* inbuf, size_t insize, 
                             byte_t* outbuf, size_t* outsize)
{    
    byte_t* outptr = outbuf;
    imli_status_t retcode = init_iscii_tables(ctx);

    if (retcode != IMLI_STATUS_OK) return retcode;

    iscii_t iscii = ctx->iscii;
    iscii->cur_lang = script;
    
    if (script != SCRIPT_ASCII)
    {
        byte_t langcode = iscii_code_from_script(script);
        *outptr++ = 0xEF;
        *outptr++ = langcode;
    }
    
    unsigned int i = 0;
    for(i = 0; i < insize; i++)
    {
        if (SYL_TERMINATOR == inbuf[i]) break;
        syl_t lch = inbuf[i];
        byte_t C = CONSONANT(lch);
        byte_t V = VOWEL(lch);
        byte_t Cj = CONJUNCT(lch);
        byte_t cur_lang = iscii->cur_lang;
        /* ASCII */
        if(IS_SYL_ASCII(lch))
        {
            *outptr++ = lch & 0xFF;
            continue;
        }

        /* special */
        if (IS_SYL_SPECIAL(lch))
        {

            //printf("At %d, special syl_t .\n");
            //printf("********** special syl_t = C, Cj, V = %d, %d, %d***************\n", C, Cj, V);
            if (Cj == 0)
            {
                switch (V)
                {
                case 15: /* 'a' changed from 63,0,4 to 63,0,15 */
                    /* 'a' (as in andrew) for bangla, kannada */
                    if (cur_lang == SCRIPT_BANGLA || cur_lang == SCRIPT_KANNADA || cur_lang == SCRIPT_HINDI || 
                        cur_lang == SCRIPT_ASAMIYA || cur_lang == SCRIPT_MARATHI)
                        *outptr++ = 0xAE;
                    
                    if (cur_lang == SCRIPT_MARATHI)
                        *outptr++ = 0xE9;
                    break;
                default:
                    //printf("*** spl char output: %c *******\n", iscii->specials[lch & 0xFF]);
                    *outptr++ = iscii->specials[lch & 0xFF];
                    break;
                }
            }
            else if (Cj == 3) // numbers
            {
                /* local language numbers. english number come under ASCII */
                *outptr++ = /*0xF1 +*/ '0' + V; // to play nice with leap office and its teleprompter
            }
            continue;
        }

        /* script switch code, emit only if the script changes */
        if (IS_SYL_SWITCH(lch))
        {
            byte_t lang = (lch & LANGCODE_MASK);
            //FIXME: unchecked access - will segfault if lang is not in range 0-11.
            if (lang != iscii->cur_lang && lang <= 12)
            {
                *outptr++ = 0xef;
                *outptr++ = lang_switch_codes[lang];
                iscii->cur_lang = lang;
            }
            continue;
        }

        //printf("*******  C = %d, V = %d\n", C, V);
        if (0 == C)
        {
            *outptr++ = iscii->vowels[V];
            continue;
        }
            
        /* handle special cases for promoted consonants */
        if (C == ksha)
        {
            /* ksha */
            *outptr++ = 0xb3;
            *outptr++ = 0xe8;
            *outptr++ = 0xd6;
        }
        else if (C == jnya)
        {
            /* jnya */
            *outptr++ = 0xba;
            *outptr++ = 0xe8;
            *outptr++ = 0xbc;
        }
        else if (C == ddra)
        {
            /* ddra */
            *outptr++ = 0xbf;
            *outptr++ = 0xe9;
        }
        else if (C == ddrha)
        {
            /* ddhra */
            *outptr++ = 0xc0;
            *outptr++ = 0xe9;
        }
        else if (C == nex)
        {
            *outptr++ = nex_iscii;
        }
        else if (C == rex)
        {
            *outptr++ = rex_iscii;
        }
        else if (C == rxl)
        {
            *outptr++ = rex_iscii;
        }
        else if (C == rxxl)
        {
            *outptr++ = rex_iscii;
        }
        else
        {
            *outptr++ = iscii->consonants[C];
        }

        if (Cj)
        {
            byte_t cj1, cj2, cj3 = 0;
            imli_is_syl_full(ctx, lch, &C, &cj1, &cj2, &cj3);

            if (cj1 == nukhta_code)
            {
                /* nukhta */
                *outptr++ = nukhta;
            }
            else 
            {
                *outptr++ = halant;
                /* handle special cases for promoted consonants */
                if (cj1 == ksha)
                {
                    /* ksha */
                    *outptr++ = 0xb3;
                    *outptr++ = 0xe8;
                    *outptr++ = 0xd6;
                }
                else if (cj1 == jnya)
                {
                    /* jnya */
                    *outptr++ = 0xba;
                    *outptr++ = 0xe8;
                    *outptr++ = 0xbc;
                }
                else if (cj1 == ddra)
                {
                    /* ddra */
                    *outptr++ = 0xbf;
                    *outptr++ = 0xe9;
                }
                else if (cj1 == ddrha)
                {
                    /* ddrha */
                    *outptr++ = 0xc0;
                    *outptr++ = 0xe9;
                }
                else if (cj1 == nex)
                {
                    *outptr++ = nex_iscii;
                }
                else if (cj1== rex)
                {
                    *outptr++ = rex_iscii;
                }
                else if (cj1 == rxl)
                {
                    *outptr++ = rex_iscii;
                }
                else if (cj1 == rxxl)
                {
                    *outptr++ = rex_iscii;
                }
                else
                {
                    *outptr++ = iscii->consonants[cj1];
                }           
            }
        
            if (cj2)
            {
                //printf("*******  Cj2 = %0x\n", consonants[cj2]);
                if (cj2 == nukhta_code)
                {
                    /* nukhta */
                    *outptr++ = nukhta;
                }
                else 
                {
                    *outptr++ = halant;
                    /* handle special cases for promoted consonants */
                    if (cj2 == ksha)
                    {
                        /* ksha */
                        *outptr++ = 0xb3;
                        *outptr++ = 0xe8;
                        *outptr++ = 0xd6;
                    }
                    else if (cj2 == jnya)
                    {
                        /* jnya */
                        *outptr++ = 0xba;
                        *outptr++ = 0xe8;
                        *outptr++ = 0xbc;
                    }
                    else if (cj2 == ddra)
                    {
                        /* ddra */
                        *outptr++ = 0xbf;
                        *outptr++ = 0xe9;
                    }
                    else if (cj2 == ddrha)
                    {
                        /* ddrha */
                        *outptr++ = 0xc0;
                        *outptr++ = 0xe9;
                    }
                    else if (cj2 == nex)
                    {
                        *outptr++ = nex_iscii;
                    }
                    else if (cj2 == rex)
                    {
                        *outptr++ = rex_iscii;
                    }
                    else if (cj2 == rxl)
                    {
                        *outptr++ = rex_iscii;
                    }
                    else if (cj2 == rxxl)
                    {
                        *outptr++ = rex_iscii;
                    }
                    else
                    {
                        *outptr++ = iscii->consonants[cj2];
                    }
                }
            }
            if (cj3)
            {
                //printf("*******  Cj2 = %0x\n", consonants[cj2]);
                if (cj3 == nukhta_code)
                {
                    /* nukhta */
                    *outptr++ = nukhta;
                }
                else 
                {
                    *outptr++ = halant;
                    /* handle special cases for promoted consonants */
                    if (cj3 == ksha)
                    {
                        /* ksha */
                        *outptr++ = 0xb3;
                        *outptr++ = 0xe8;
                        *outptr++ = 0xd6;
                    }
                    else if (cj3 == jnya)
                    {
                        /* jnya */
                        *outptr++ = 0xba;
                        *outptr++ = 0xe8;
                        *outptr++ = 0xbc;
                    }
                    else if (cj3 == ddra)
                    {
                        /* ddra */
                        *outptr++ = 0xbf;
                        *outptr++ = 0xe9;
                    }
                    else if (cj3 == ddrha)
                    {
                        /* ddrha */
                        *outptr++ = 0xc0;
                        *outptr++ = 0xe9;
                    }
                    else if (cj3 == nex)
                    {
                        *outptr++ = nex_iscii;
                    }
                    else if (cj3 == rex)
                    {
                        *outptr++ = rex_iscii;
                    }
                    else if (cj3 == rxl)
                    {
                        *outptr++ = rex_iscii;
                    }
                    else if (cj3 == rxxl)
                    {
                        *outptr++ = rex_iscii;
                    }
                    else
                    {
                        *outptr++ = iscii->consonants[cj3];
                    }
                }
            }
        }
        if ((V > 0) && (V != 15))
        {
                *outptr++ = iscii->vowels_m_form[V];
        }
        else if (V == 15)
        {
            *outptr++ = halant;
            *outptr++ = halant;
        }
    }
    
    *outsize = outptr - outbuf;
    return IMLI_STATUS_OK;
}
