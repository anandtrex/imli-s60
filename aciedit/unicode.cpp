/*************************************************************
 * unicode.cpp
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#include "TextEdit.h"
#include "libimli.h"
#include <QTextCodec>
#include <QFile>

/* unicode conversion stuff */
static QTextCodec* GetCodec(imli_script_t script)
{
    switch (script)
    {
    case SCRIPT_HINDI:
    case SCRIPT_MARATHI:
        return QTextCodec::codecForName("Iscii-Dev");
    case SCRIPT_TAMIL:
        return QTextCodec::codecForName("Iscii-Tml");
    case SCRIPT_TELUGU:
        return QTextCodec::codecForName("Iscii-Tel");
    case SCRIPT_KANNADA:
        return QTextCodec::codecForName("Iscii-Knd");
    case SCRIPT_MALAYALAM:
        return QTextCodec::codecForName("Iscii-Mlm");
    case SCRIPT_ORIYA:
        return QTextCodec::codecForName("Iscii-Ori");
    case SCRIPT_BANGLA:
        return QTextCodec::codecForName("Iscii-Bng");
    case SCRIPT_GUJARATI:
        return QTextCodec::codecForName("Iscii-Gjr");
    case SCRIPT_PUNJABI:
        return QTextCodec::codecForName("Iscii-Pnj");
    case SCRIPT_ASCII:
        return QTextCodec::codecForName("latin1");
        
        /* other scripts are unsupported */
    default: 
        return 0;
    }
}

int SaveUTF8(TextEdit* editor, QString filename)
{
    if (!editor) 
        return -1;

    TextStorage* text = editor->GetTextStorage();
    if (text->Size() <= 0)
        return -1;
    
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) 
        return -1;
    
    
    TextStorage::Selection sel = text->GetText();
    QByteArray acidata;
    while (sel.IsValid())
    {
        /* check if the script has a corresponding QTextCodec  */
        imli_script_t script = sel.GetScript();
        QTextCodec* codec = GetCodec(script);
        if (!codec)
        {
            sel.NextRun();
            continue;
        }
        
        SylString s = sel.GetData();
        size_t insize = s.size();
        /* 16 bytes per syllable - should be sufficient for upto 4 levels of conjuncts */
        size_t outsize = insize*16;
        acidata.resize(outsize);
        imli_to_iscii(text->GetContext(), script, s.data(), insize, (byte_t*)acidata.data(), &outsize);
        acidata.resize(outsize);

        /* convert iscii to utf8 using QTextCodec */
        QString unicode = codec->toUnicode(acidata);
        QByteArray utf8 = unicode.toUtf8();
        file.write(utf8);

        sel.NextRun();
    }
    file.close();
    return 0;
}
