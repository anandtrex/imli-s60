/*************************************************************
 * ListBox.h
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#ifndef __LISTBOX__
#define __LISTBOX__

#include <QtGui>
#include <vector>
#include <string>
#include <libimli.h>

typedef std::basic_string<syl_t> SylString;

class ListItem : public QListWidgetItem
{
public:
    ListItem(imli_context_t ctx, SylString s, QListWidget* parent = 0);
    SylString GetText() { return str_; }
    
    
protected:
    void SylsToGlyphs();
    imli_context_t ctx_;
    SylString str_;
};

class ListBox : public QListWidget
{
public:
    ListBox(imli_context_t, std::vector<SylString>&, QTextCharFormat&, QWidget* parent = 0);
    ~ListBox();
protected:
    void focusOutEvent(QFocusEvent*);
    void keyPressEvent(QKeyEvent*);
    
private:
    void CreateItems();
    imli_context_t ctx_;
    const std::vector<SylString>& data_;
    QTextCharFormat format_;
};

#endif
