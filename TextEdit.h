/*************************************************************
 * TextEdit.h
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#ifndef TEXTEDIT_H_
#define TEXTEDIT_H_

#include <QTextEdit>
#include <QVector>
#include <QRegExp>

#include <libimli.h>

#include "TextStorage.h"

class QLabel;
class Dictionary;
class QListWidgetItem;

class TextEdit : public QTextEdit
{
Q_OBJECT

public:
    TextEdit(imli_context_t context, std::vector<QTextCharFormat>&, QWidget* parent = 0);
    virtual ~TextEdit();

    TextStorage* GetTextStorage(){ return text_; }
    void ToRTF(QString filename);
    void SetStyles(const std::vector<QTextCharFormat>& src);

public slots:
    void Update();
    void Undo();
    void Cut();
    void Copy();
    void Paste();
    void PasteAsASCII();
    void Clear();
    void SelectAll();
    void Transliterate(imli_script_t);
    void ZoomIn(int range = 2);
    void ZoomOut(int range = 2);
    void SyncCursors();
    void InsertItem(QListWidgetItem*);
    
protected:
    enum SelectionOp {CutSelection = 1, CopySelection, ClearSelection};
    void ProcessSelection(SelectionOp op);

    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);
    bool ProcessKey(QString text);
    int SelectionToGlyphs(const TextStorage::Selection&, QByteArray&);
    void DumpState();
protected:
    imli_context_t context_;
    std::vector<QTextCharFormat> styles_;
    TextStorage* text_;
    static TextStorage* clipboard_;
    bool standalone_vowel_requested_;
protected slots:
    void slotTextModified(const TextStorage::Event& event);
protected:
    QLabel* statLabel_;
    bool showStats_;
public:
    void setStatisticsLabel(QLabel* label);
public slots:
    void EnableStats(bool);
    void ShowStats();
protected:
    int cursorPos_;
public slots:
    void RecordUndo();
    void ClearUndo();
protected:
    QRegExp wcexp_;
public:
    int WordCount();
    void AddSelectionToDictionary();
    int ShowCompletions();
    static Dictionary* dict_;

protected:
    void AddAllWordsToDictionary(const SylString&);
};

#endif /*TEXTEDIT_H_*/
