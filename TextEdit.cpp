/*************************************************************
 * TextEdit.cpp
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#include <QtGui>
#include <QByteArray>
#include "libimli.h"
#include "aciedit.h"
#include "Dictionary.h"
#include "ListBox.h"
#include "TextEdit.h"
#include "TextEditInputContext.h"

#define alive(msg) \
    QMessageBox::information(0, QString("DEBUG:"), QString("At file %1 , line: %2 : %3").arg(__FILE__).arg(__LINE__).arg((msg)));

TextStorage* TextEdit::clipboard_ = 0;
Dictionary* TextEdit::dict_ = 0;

TextEdit::TextEdit(imli_context_t context, std::vector<QTextCharFormat>& styles, QWidget* parent)
    : QTextEdit(parent), styles_(styles), standalone_vowel_requested_(false), showStats_(false), wcexp_("(\\w+)\\b")
{
    context_ = context;
    text_ = new TextStorage(context_);
    if (!clipboard_)
    	clipboard_ = new TextStorage(context_);
    if (!dict_)
    	dict_ = new Dictionary();
    
    connect(text_, SIGNAL(TextModified(const TextStorage::Event&)), this, SLOT(slotTextModified(const TextStorage::Event&)));
    connect(text_, SIGNAL(UndoRecorded()), this, SLOT(RecordUndo()));
    connect(text_, SIGNAL(UndoCleared()), this, SLOT(ClearUndo()));
    //connect(text_, SIGNAL(InvalidState(QString)), this, SLOT(SyncCursors()));
    
    //For preventing VKB from coming up
    TextEditInputContext *textEditInputContext = new TextEditInputContext();
    this->setInputContext(textEditInputContext);
    
    //Enable word wrap
    //this->setWordWrapMode(QTextOption::WordWrap);
    
}

TextEdit::~TextEdit()
{
    delete text_;
}

void TextEdit::Update()
{
    this->clear();
    TextStorage::Selection sel = text_->GetText();
    QTextCursor cursor = this->textCursor();

    while (sel.IsValid())
    {
        SylString s = sel.GetData();
        imli_script_t script = sel.GetScript();
        QByteArray glyphs;
        glyphs.reserve(s.size()*8);

        size_t len = 0;
        imli_status_t status =
            syls_to_glyphs(context_,
                           s.data(),
                           s.size(),
                           script,
                           (byte_t*)glyphs.data(),
                           glyphs.capacity(),
                           &len);

        if (status == IMLI_STATUS_OK)
        {
            glyphs.resize(len);
            cursor.insertText(QString::fromAscii(glyphs, glyphs.size()), styles_[int(script)]);
        }
        else
        {
            QMessageBox::critical(this, tr("Critical Error..."), tr("Glyph generation failed."));
        }
        sel.NextRun();
    }
    cursor.setPosition(0);
    text_->MoveTo(0, TextStorage::Syllables);
    this->setTextCursor(cursor);    
}

void TextEdit::ToRTF(QString filename)
{
    QTextDocument* doc = this->document();
    if (!doc)
    {
        QMessageBox::information(this, tr("Info:"), tr("Nothing to export"));
        return;
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("Error:"), tr("Unable to open file %1 for writing").arg(filename));
        return;
    }
    QTextStream out(&file);

    QVector<QTextFormat> styles = doc->allFormats();

    /* header information */
    out << "{\\rtf1\\ansi\\ansicpg1252";

    QString fonttbl = "{\\fonttbl";

    for(int i = 0; i < styles.size(); i++)
    {
        QTextFormat format = styles[i];
        if (format.isCharFormat())
        {
            QTextCharFormat cformat = format.toCharFormat();
            QString font_family = cformat.fontFamily();
            // hack required for CDAC's ISFOC fonts eg: DV TTYogesh is actually DV-TTYogesh
            font_family.replace(' ', '-');
            fonttbl.append(QString("{\\f%1 %2;}").arg(i).arg(font_family));
        }
    }
    out << fonttbl << "}";

    QString hex("\\'%1");
    QTextBlock block = doc->begin();
    while (block.isValid())
    {
        out << "\\par";
        QTextBlock::iterator it;
        for(it = block.begin(); !(it.atEnd()); ++it)
        {
            QTextFragment fragment = it.fragment();
            out << "{\\f" << fragment.charFormatIndex() << "\\fs" << fragment.charFormat().fontPointSize()*2; // \fsN is font size in "half points"
            QString text = fragment.text();
            for(int i = 0; i < text.size(); i++)
            {
                out << hex.arg(text[i].unicode(), 0, 16);
            }
            out << "}";
        }
        block = block.next();
    }
    out << "}";
}

void TextEdit::mousePressEvent(QMouseEvent* event)
{
    QTextEdit::mousePressEvent(event);
    SyncCursors();
}

void TextEdit::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();

    if (key > Qt::Key_Space && key <= Qt::Key_AsciiTilde && !(event->modifiers() & Qt::ControlModifier))
    {
        QString txt = event->text();
        if(ProcessKey(txt)) 
            event->accept();
        return;
    }

    if (key >= Qt::Key_Pause && key <= Qt::Key_Clear)
    {
        qDebug() << "forwarding to base class...";
        QTextEdit::keyPressEvent(event);
        return;
    }

    imli_script_t script = imli_get_active_script(context_);
    QTextCursor cursor(this->textCursor());

    if (key == Qt::Key_V && ((event->modifiers() & Qt::ControlModifier) &&
                             (event->modifiers() & Qt::ShiftModifier)))
    {
        this->PasteAsASCII();
        goto accept;
    }

    switch (key)
    {
    case Qt::Key_Escape:
        event->ignore();
        return;
    case Qt::Key_F5:
    {
        /* refresh */
        int pos = cursor.position();
        this->Update();
        text_->MoveTo(pos);
        cursor.setPosition(pos);
        this->setTextCursor(cursor);
        goto accept;
    }
    case Qt::Key_F6:
        this->DumpState();
        goto accept;
    case Qt::Key_F7:
        SyncCursors();
        goto accept;
    case Qt::Key_F9:
        event->ignore();
        return;
    case Qt::Key_Return:
    {
        syl_t syl = SYL_NEWLINE;
        byte_t width = 0;
        syl_width(context_, syl, script, &width);
        text_->Insert(script, &syl, 1);
        goto accept;
    }
    case Qt::Key_Space:
    {
        syl_t syl = SYL_SPACE;
        text_->Insert(script, &syl, 1);
        goto accept;
    }
    case Qt::Key_Tab:
    {
        syl_t syl = SYL_TAB;
        text_->Insert(script, &syl, 1);
        goto accept;
    }
    case Qt::Key_Backspace:
    {
        if (cursor.hasSelection())
        {
            this->Clear();
            goto accept;
        }
            
        //TODO: implement the sub syllable editing feature
        if (cursor.atStart())
        {
            QApplication::beep();
        }
        else
        {
            int d = text_->DeletePreviousSyl();
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, d);
            cursor.removeSelectedText();
        }
        goto accept;
    }
    case Qt::Key_Delete:
    {
	if (cursor.hasSelection())
        {
            this->Clear();
            goto accept;
        }
        
        if (cursor.atEnd())
        {
            QApplication::beep();
        }
        else 
        {
            int d = text_->DeleteSyl();
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, d);
            cursor.removeSelectedText();
        }
        goto accept;
    }
    default:
        ;
    }
    if (false)
    {
    }
    else if (event == QKeySequence::MoveToNextChar)
    {
        int d = text_->MoveBy(1);
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, d);
        this->setTextCursor(cursor);
        goto accept;
    }
    else if (event == QKeySequence::MoveToPreviousChar)
    {
        int d =  0;
        d = text_->MoveBy(1, TextStorage::Backward);
        cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, abs(d));
        this->setTextCursor(cursor);
        goto accept;
    }
    else if (event == QKeySequence::SelectNextChar)
    {
        int d = text_->MoveBy(1);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, d);
        this->setTextCursor(cursor);
        goto accept;
    }
    else if (event == QKeySequence::SelectPreviousChar)
    {
        int d =  0;
        d = text_->MoveBy(1, TextStorage::Backward);
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, abs(d));
        this->setTextCursor(cursor);
        goto accept;
    }
    else if (event == QKeySequence::Cut)
    {
        this->Cut();
    }
    else if (event == QKeySequence::Copy)
    {
        this->Copy();
    }
    else if (event == QKeySequence::Paste)
    {
        this->Paste();
    }
    else if (event == QKeySequence::Undo)
    {
        this->Undo();
    }
    else
    {        
        for (int i = (int)QKeySequence::MoveToNextWord; i < (int)QKeySequence::DeleteEndOfLine; i++)
        {
            if (event == (QKeySequence::StandardKey)i)
            {
                QTextEdit::keyPressEvent(event); 
                SyncCursors();
                goto accept;
            }
        }
    }

accept:
    event->accept();
    return;    
}

void TextEdit::SyncCursors()
{
    QTextCursor cursor = this->textCursor();
    if (cursor.atStart())
    {
        text_->MoveTo(0);
    }
    else if (cursor.atEnd()) 
    {
        text_->MoveTo(text_->Size(), TextStorage::Syllables);
    }
    else
    {
        int position = cursor.position();
        int delta = text_->MoveTo(position, TextStorage::Glyphs);
        cursor.setPosition(position + delta, (cursor.hasSelection() ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor));
        this->setTextCursor(cursor);
    }
    DumpState();
}

bool TextEdit::ProcessKey(QString text)
{
    char key = text.toAscii()[0];
    QTextCursor cursor = this->textCursor();
    int delPrevSyl = 0;
    imli_script_t prevScript = SCRIPT_UNSUPPORTED;
    syl_t prevSyl = SYL_INVALID;
    bool retval = false;

    if (cursor.position())
    {
        if (text_->GetPrevSyl(&prevScript, &prevSyl) != 0)
        {
            qWarning() << "Error!: failed to get prev syl.\nPREV: script = " << prevScript << " syl = " << prevSyl;
        }
    }

    /* kludgy but required for non joining vowels in phonetic input
     * method. FIXME: try to move it to libimli2.
     */
    
    if (imli_get_active_input_method(context_) == PHONETIC &&
        text == "\\")
    {
        qDebug("setting standalone flag...");
        standalone_vowel_requested_ = !standalone_vowel_requested_;
        return true;
    }
        
    imli_script_t script = imli_get_active_script(context_);
    syl_t syl = SYL_INVALID;
    if (script == SCRIPT_ASCII)
        syl = (ASCII_START | key);
    else
        syl = imli_process_key(context_, 
                               (char)key, 
                               prevScript,
                               prevSyl,
                               (standalone_vowel_requested_ ? FLAG_STANDALONE_VOWEL : FLAG_NONE), 
                               &delPrevSyl);
 
    if (syl != SYL_INVALID)
    {
        if (standalone_vowel_requested_)
            standalone_vowel_requested_ = false;
        
        if (delPrevSyl)
        {
            text_->Substitute(script, syl);
        }
        else
        {
            qDebug() << "SYL = [" << CONSONANT(syl) << ", " << CONJUNCT(syl) << ", " << VOWEL(syl) << "]";
            int n = text_->Insert(script, &syl, 1);
            if (n == -1) 
                qDebug() << "Insertion failed!!!";
        }
        retval = true;
    }
    else
    {
        qDebug() << prevSyl << " " << prevScript;
        QApplication::beep();
    }
    return retval;
}

int TextEdit::SelectionToGlyphs(const TextStorage::Selection& sel,
                                QByteArray& glyphs)
{
    if (!sel.IsValid()) return -1;

    SylString s = sel.GetData();
    glyphs.clear();
    glyphs.reserve(s.size()*8);
    imli_script_t script = sel.GetScript();
    size_t len = 0;
    imli_status_t status =
        syls_to_glyphs(context_,
                       s.data(),
                       s.size(),
                       script,
                       (byte_t*)glyphs.data(),
                       glyphs.capacity(),
                       &len);
    
    if (status == IMLI_STATUS_OK)
    {
        glyphs.resize(len);
        return len;
    }
    return -1;
}

void TextEdit::slotTextModified(const TextStorage::Event& event)
{
    if (event.action == TextStorage::Removal)
    {
        QTextCursor cursor(this->textCursor());
        qDebug() << "Remove: pos = " << cursor.position() << " length = " << event.nglyphs;
        cursor.movePosition((event.dir == TextStorage::Forward ? QTextCursor::Right : QTextCursor::Left),
                            QTextCursor::KeepAnchor, 
                            event.nglyphs);
        cursor.removeSelectedText();
        return;
    }
    else if (event.action == TextStorage::Replacement)
    {
        QTextCursor cursor(this->textCursor());
        cursor.beginEditBlock();
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, event.nglyphs);
        cursor.removeSelectedText();
        cursor.insertText(QString::fromAscii(event.chunk.glyphs, event.chunk.glyphs.size()), styles_[int(event.chunk.script)]);
        cursor.endEditBlock();
        this->setTextCursor(cursor);
    }
    else if (event.action == TextStorage::Insertion)
    {
        QTextCursor cursor(this->textCursor());
        cursor.beginEditBlock();
        cursor.insertText(QString::fromAscii(event.chunk.glyphs, event.nglyphs), styles_[int(event.chunk.script)]);
        cursor.endEditBlock();
        this->setTextCursor(cursor);
    }
    QTextEdit::repaint();
}

void TextEdit::DumpState()
{
    if (!statLabel_ || !showStats_)
        return;
    
    QTextCursor cursor(this->textCursor());
    // statLabel_->setText(QString("cursor_pos = <b>%1</b> syllable_pos = <b>%2</b> syls = <b>%3</b> glyphs = <b>%4</b>")
//                         .arg(cursor.position())
//                         .arg(text_->GetInsertPos())
//                         .arg(text_->Size())
//                         .arg(this->toPlainText().size()));
    
    
    //qDebug() << "STATE: cursor_pos = " << cursor.position() << " syllable_pos = " << text_->GetInsertPos() << " syllables = " << text_->Size() << " glyph_length = \n\n";
    text_->Dump();    
//    qDebug() << "CLIPBOARD ";
//     qDebug() << "length = " << clipboard_->Size();
//     clipboard_->Dump();
}

void TextEdit::Cut()
{
    this->ProcessSelection(TextEdit::CutSelection);
}

void TextEdit::Copy()
{
    this->ProcessSelection(TextEdit::CopySelection);
}

void TextEdit::Paste()
{
    if (clipboard_->Size() <= 0)
    {
	QApplication::beep();
        return;
    }
    
    // clear selection if any
    if (this->textCursor().hasSelection())
        ProcessSelection(TextEdit::ClearSelection);
    
    TextStorage::Selection sel = clipboard_->GetText();

    QByteArray glyphs;
    
    while (sel.IsValid())
    {
	imli_script_t script = sel.GetScript();
	SylString s = sel.GetData();
	text_->Insert(script, &s[0], s.size());
	sel.NextRun();
    }
}

void TextEdit::Clear()
{
    this->ProcessSelection(TextEdit::ClearSelection);
}

void TextEdit::SelectAll()
{
    QTextCursor cursor(this->textCursor());
    cursor.select(QTextCursor::Document);
    this->setTextCursor(cursor);
}

void TextEdit::ProcessSelection(TextEdit::SelectionOp op)
{
    QTextCursor cursor(this->textCursor());
    if (!cursor.hasSelection())
    {
        QApplication::beep();
        return;
    }
    
    int sel_start = cursor.selectionStart();
    int sel_end   = cursor.selectionEnd();
    //qDebug() << "cursor start = " << sel_start << " cursor end = "  << sel_end;
    
    text_->MoveTo(sel_start, TextStorage::Glyphs, TextStorage::Mark);
    text_->MoveTo(sel_end, TextStorage::Glyphs, TextStorage::Point);
    
    if (op != TextEdit::ClearSelection)
    {
	TextStorage::Selection sel = text_->GetSelection();
	if (!sel.IsValid())
	{
	    //qDebug() << "OOPS: selection invalid!!!";
	    return;
	}
	
        QString glyphs = cursor.selectedText();
        QClipboard* system_clipboard = QApplication::clipboard();
        system_clipboard->setText(glyphs);
        if (system_clipboard->supportsSelection())
            system_clipboard->setText(glyphs, QClipboard::Selection);
        
	clipboard_->Clear();

	while (sel.IsValid())
	{
	    imli_script_t script = sel.GetScript();
	    SylString s = sel.GetData();
	
	    qDebug() << ">>>>>>>>>> script = " << script << " sylstring.size = " << s.size();
	    clipboard_->Insert(script, s.data(), s.size());
	    
	    sel.NextRun();
	}
    }
    
    if (op != TextEdit::CopySelection)
    {
	text_->RemoveSelection();
	cursor.removeSelectedText();
    }
}

/*!
  \brief Convert syllables in the current selection to \a script
*/
void TextEdit::Transliterate(imli_script_t new_script)
{
    QTextCursor cursor(this->textCursor());
    if (!cursor.hasSelection())
        return;
    
    int sel_start = cursor.selectionStart();
    int sel_end   = cursor.selectionEnd();
    //qDebug() << "sel start = " << sel_start << " end = "  << sel_end;
    
    text_->MoveTo(sel_start, TextStorage::Glyphs, TextStorage::Mark);
    text_->MoveTo(sel_end, TextStorage::Glyphs, TextStorage::Point);
    
    TextStorage::Selection sel = text_->GetSelection();
    if (!sel.IsValid())
    {
        //qDebug() << "OOPS: selection invalid!!!";
        return;
    }
    
    clipboard_->Clear();
    
    while (sel.IsValid())
    {
        imli_script_t script = sel.GetScript();
        if (script != SCRIPT_ASCII)
            script = new_script;
        
        SylString s = sel.GetData();
	
        //qDebug() << ">>>>>>>>>> script = " << script << " sylstring.size = " << s.size();
        clipboard_->Insert(script, &s[0], s.size());
	
        sel.NextRun();
    }
    
    text_->RemoveSelection();
    cursor.removeSelectedText();
    this->Paste();
}

void TextEdit::contextMenuEvent(QContextMenuEvent* event)
{
    ACIEdit* main_window = dynamic_cast<ACIEdit*>(QApplication::activeWindow());
    if (main_window)
        main_window->PopupEditMenu(event);
}

void TextEdit::ZoomIn(int range)
{
    for(unsigned int i = 0; i < styles_.size(); i++)
        styles_[i].setFontPointSize(styles_[i].fontPointSize() + range);

    int pos = this->textCursor().position();
    this->Update();
    QTextCursor c(this->textCursor());
    c.setPosition(pos);
    this->setTextCursor(c);
    this->SyncCursors();
}

void TextEdit::ZoomOut(int range)
{
    for(unsigned int i = 0; i < styles_.size(); i++)
        styles_[i].setFontPointSize(styles_[i].fontPointSize() - range);

    int pos = this->textCursor().position();
    this->Update();
    QTextCursor c(this->textCursor());
    c.setPosition(pos);
    this->setTextCursor(c);
    this->SyncCursors();
}

void TextEdit::setStatisticsLabel(QLabel* label)
{
    statLabel_ = label;
}

void TextEdit::EnableStats(bool state)
{
    showStats_ = state;
    if (showStats_ && statLabel_)
    {
        connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(ShowStats()));
        ShowStats();
    }
    else
    {
        disconnect(this->document(), SIGNAL(cursorPositionChanged()), this, SLOT(ShowStats()));
    }
}

void TextEdit::ShowStats()
{
    //DumpState();
    int wc = WordCount();
    statLabel_->setText(QString("Words: %1").arg(wc));
}

void TextEdit::PasteAsASCII()
{
    QString type("plain");
    QString data = QApplication::clipboard()->text(type, QClipboard::Clipboard);
    if (data.isEmpty())
    {
        QApplication::beep();
        return;
    }
    
    QByteArray asc = data.toAscii();        
    QVector<syl_t> s(asc.size());

    for (int i = 0; i < s.size(); i++)
        s[i] = syl_t(ASCII_START | asc.at(i));
    
    if (text_->HasSelection())
        this->ProcessSelection(ClearSelection);
    
    text_->Insert(SCRIPT_ASCII, &s[0], s.size());
}

void TextEdit::SetStyles(const std::vector<QTextCharFormat>& src)
{
    for(size_t i = 0; i < src.size(); i++)
        styles_[i] = src[i];
}

void TextEdit::Undo()
{
    if (text_->CanUndo())
    {
        QTextCursor c = this->textCursor();
        int pos = cursorPos_;
        c.setPosition(pos);

        TextStorage::Selection sel = text_->Undo();
        QByteArray glyphs;

        while (sel.IsValid())
        {
	    if(SelectionToGlyphs(sel, glyphs) != -1)
            {
                c.insertText(QString::fromAscii(glyphs, glyphs.size()), styles_[int(sel.GetScript())]); 
            }
            sel.NextRun();
        }

        c.setPosition(pos);
        this->setTextCursor(c);
    }
}

void TextEdit::RecordUndo()
{
    QTextCursor c = this->textCursor();
    if (c.hasSelection())
        cursorPos_ = c.selectionStart();
    else
        cursorPos_ = c.position();
}

void TextEdit::ClearUndo()
{
    cursorPos_ = -1;
}

int TextEdit::WordCount()
{
    QString txt = this->toPlainText();
    int pos = 1;
    int counter = 0;

    while (pos >= 0)
    {
        pos = wcexp_.indexIn(txt, pos);
        if (pos != -1)
        { 
            counter++;
            pos += wcexp_.matchedLength();
        }
    }

    return counter;
}

void TextEdit::AddAllWordsToDictionary(const SylString& str)
{
    std::vector<syl_t> buf;
    for (size_t j = 0; j < str.length(); j++)
    {
	syl_t syl = str[j];
	if (!IS_SYL_SPECIAL(syl) && !IS_SYL_ASCII(syl))
	{
	    buf.push_back(syl);
	}
	else
	{
	    if (buf.size())
	    {
		dict_->Insert(&buf[0], buf.size());
		buf.clear();
	    }
	}
    }
    if (buf.size())
    {
	dict_->Insert(&buf[0], buf.size());
    }
}

void TextEdit::AddSelectionToDictionary()
{
    //FIXME: check for selection and add only selected words (ignore ASCII)
    TextStorage::Selection sel = text_->GetText(0, text_->Size());
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    ACIEdit* mainWindow = dynamic_cast<ACIEdit*>(qApp->activeWindow());    
    if (mainWindow)
    {
	mainWindow->statusBar()->showMessage("Indexing words...");
	qApp->processEvents();
    }
    int i = 0;
    
    while (sel.IsValid())
    {
	qDebug() << "Processing run #" << ++i << " script = " << sel.GetScript();
	
	if (sel.GetScript() != SCRIPT_ASCII)
	    AddAllWordsToDictionary(sel.GetData());
	sel.NextRun();
    }
    
    if (mainWindow)
	mainWindow->statusBar()->clearMessage();
    QApplication::restoreOverrideCursor();
}

int TextEdit::ShowCompletions()
{
    SylString s = text_->GetWord();
    if (s.size())
    {
	//s.append(1, SYL_INVALID);
	std::vector<SylString> results;
	int n = dict_->MatchPrefix(s.data(), s.size(), results);
	if (n)
	{
	    QTextCharFormat format = styles_[(int)imli_get_active_script(context_)];
	    ListBox* box = new ListBox(context_, results, format, qApp->activeWindow());
	    connect(box, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(InsertItem(QListWidgetItem*)));
	    connect(box, SIGNAL(itemActivated(QListWidgetItem*)), box, SLOT(deleteLater()));
	    
	    box->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	    QPoint point = qApp->activeWindow()->mapFromGlobal(this->mapToGlobal(cursorRect().bottomRight()));
	    box->move(point);
	    box->show();
	    box->setFocus(Qt::OtherFocusReason);
	
	    qApp->processEvents();
	}
	return n;
    }
    return -1;
}

void TextEdit::InsertItem(QListWidgetItem* item)
{
    ListItem* it = dynamic_cast<ListItem*>(item);
    if (!it)
	return;
    SylString str = it->GetText();
    if (!str.size())
	return;
    
    imli_script_t script = imli_get_active_script(context_);
    text_->Insert(script, str.data(), str.size());
    setFocus(Qt::OtherFocusReason);
}
