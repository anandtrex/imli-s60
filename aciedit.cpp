/*************************************************************
 * aciedit.cpp
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
#include <QtNetwork>

#include "version.h"

#include "aciedit.h"
#include "CentralWidget.h"
#include "TextEdit.h"
//#include "printpreview.h"
#include "keyboard.h"
#include "Dictionary.h"

#define alive(msg) \
    QMessageBox::information(0, QString("DEBUG:"), QString("At file %1 , line: %2 : %3").arg(__FILE__).arg(__LINE__).arg((msg)));


ACIEdit::ACIEdit(imli_context_t context, QWidget* parent)
    : QMainWindow(parent), styles_(int(SCRIPT_ASCII) + 1), context_(context), id_(1), lastScript_(SCRIPT_HINDI)
{
    CreateActions();
    CreateMenus();
    CreateToolBars();
    CreateStatusBar();
   
    
    connect(cmbScripts_, SIGNAL(currentIndexChanged(int)), this, SLOT(ScriptChanged(int)));
    connect(cmbInputMethods_, SIGNAL(currentIndexChanged(int)), this, SLOT(InputMethodChanged(int)));

    CentralWidget* centralWidget = new CentralWidget(this);
    virtualKeyboard_ = centralWidget->virtualKeyboard;
    
    tab_ = centralWidget->tabWidget;
    txtFind_ = centralWidget->txtFind;
    chkWholeWords_ = centralWidget->chkWholeWords;
    findFrame_ = centralWidget->findFrame;
    connect(findAct_, SIGNAL(triggered()), findFrame_, SLOT(show()));
    connect(findAct_, SIGNAL(triggered()), txtFind_, SLOT(setFocus()));
    connect(showKeyboardAct_, SIGNAL(toggled(bool)), (QWidget*)virtualKeyboard_, SLOT(setVisible(bool)));
    
    connect(tab_, SIGNAL(currentChanged(int)), this, SLOT(UpdateTitleBar()));
    QToolButton* closeButton = new QToolButton(tab_);
    closeButton->setIcon(QIcon(":/images/closetab.png"));
    closeButton->setAutoRaise(true);
    connect(closeButton, SIGNAL(clicked()),this, SLOT(CloseDoc()));
    tab_->setCornerWidget(closeButton);
    setCentralWidget(centralWidget);
    
    ReadSettings();
    
    ScriptChanged(cmbScripts_->currentIndex());
    InputMethodChanged(cmbInputMethods_->currentIndex());
    
    txtFind_->SetStyles(styles_);
    txtFind_->ZoomOut(8);
    
    NewDoc();
}

ACIEdit::~ACIEdit()
{
}

void ACIEdit::InitContext()
{

	QString privatePathQt(QDir::currentPath());
	QString privatePathSymbian(QDir::toNativeSeparators(privatePathQt));
	const char* directory = privatePathSymbian.toLatin1();
	
	
    if (imli_context_new(&context_, directory) != IMLI_STATUS_OK)
    {
        QMessageBox::critical(0, tr("Fatal Error:"), tr("Failed to initialize IMLI context. Exiting..."));
        ::exit(42);
    }
    
    if (init_iscii_tables(context_) != IMLI_STATUS_OK)
    {
        QMessageBox::critical(0, tr("Fatal Error:"), tr("Failed to initialize ISCII tables. Exiting..."));
        ::exit(42);    
    }
}

void ACIEdit::closeEvent(QCloseEvent *event)
{
    while (tab_->count())
    {
        if (!CloseDoc())
        {
            event->ignore();
            return;
        }
    }
    
    WriteSettings();
    event->accept();
}

TextEdit* ACIEdit::GetActiveEditor()
{
    return dynamic_cast<TextEdit*>(tab_->currentWidget());
}

TextEdit* ACIEdit::NewTab()
{
    TextEdit* editor = new TextEdit(context_, styles_, 0);
    editor->setStatisticsLabel(statLabel_);
    connect(editor->GetTextStorage(), SIGNAL(InvalidState(QString)), this, SLOT(FlagBadState(QString)));
    editor->EnableStats(showStatsAct_->isChecked());
    tab_->addTab(editor, QString("Untitled%1").arg(id_++));
    tab_->setCurrentIndex(tab_->count() - 1);
    tab_->cornerWidget(Qt::TopRightCorner)->setEnabled(tab_->count() > 0);
    
    //To set proper size
    QDesktopWidget *qdw = QApplication::desktop();
    QRect rect = qdw->availableGeometry();
    QSize size = QSize();
    size.setWidth(rect.width());
    size.setHeight(rect.height());
    editor->setMaximumSize(size);
    return editor;
}

void ACIEdit::NewDoc()
{
    TextEdit* editor = NewTab();

    connect(showStatsAct_, SIGNAL(triggered(bool)), editor, SLOT(EnableStats(bool)));
    connect(editor->document(), SIGNAL(contentsChanged()), this, SLOT(DocumentWasModified()));
    
    UpdateTitleBar();
    statusBar()->showMessage(tr("Ready."));
}

/* defined in aci.cpp */
int LoadISCII(TextEdit* editor, QString filename, imli_script_t default_script);

void ACIEdit::OpenDoc()
{
    QAction* action = dynamic_cast<QAction*>(sender());
    if(action)
    {
        if (action == openAct_)
            OpenDoc("");
        else
            OpenDoc(action->text()); // called by actions in recent files menu.
    }
}

void ACIEdit::OpenDoc(QString filename)
{
    if (filename.isEmpty())
    {
        filename = QFileDialog::getOpenFileName(this, tr("Open File..."), lastOpenDir_, tr("ISCII Files (*.aci)"));
    }
    
    if (!filename.isEmpty())
    {
        QFileInfo info(filename);
        lastOpenDir_ = info.absolutePath();

        int tabindex = CheckTabs(filename);
        if (tabindex != -1)
        {
            tab_->setCurrentIndex(tabindex);
            return;
        }
        
        TextEdit* editor = NewTab();
        if (!editor)
        {
            QMessageBox::critical(this, tr("Critical Error:"),
                                  tr("Widget creation error!"));
            return;
        }
        TextStorage* text = editor->GetTextStorage();

        text->EnableNotification(false);
        statusBar()->showMessage(tr("Converting ISCII..."));
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        imli_script_t cur_script = imli_get_active_script(context_);
        
        if (LoadISCII(editor, filename, cur_script) != 0)
        {
            QMessageBox::critical(this, tr("Critical Error:"),
                                  tr("Failed to open input file: %1").arg(filename));
            CloseDoc();
            return;
        }
        
        statusBar()->showMessage(tr("Rendering..."));
        editor->Update();
        imli_select_script(context_, cur_script);
        
        text->EnableNotification(true);
        statusBar()->clearMessage();
        editor->setDocumentTitle(filename);
        editor->document()->setModified(false);
        
        tab_->setTabText(tab_->currentIndex(), info.fileName());

        connect(showStatsAct_, SIGNAL(triggered(bool)), editor, SLOT(EnableStats(bool)));
        connect(editor->document(), SIGNAL(contentsChanged()), this, SLOT(DocumentWasModified()));

        UpdateTitleBar();
        statusBar()->showMessage(tr("Loaded."), 2000);
        AddToFileHistory(filename);
	QApplication::restoreOverrideCursor();
    }
}

void ACIEdit::UpdateTitleBar()
{
    QTextEdit* editor = (QTextEdit*)GetActiveEditor();
    if (editor)
    {
        if (editor->documentTitle().isEmpty())
            this->setWindowTitle(tr("%1[*] - Acharya-v%2.%3 (build %4)").arg(tab_->tabText(tab_->currentIndex())).arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(BUILD));
        else
            this->setWindowTitle(tr("%1[*] - Acharya-v%2.%3 (build %4)").arg(editor->documentTitle()).arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(BUILD));
    }
    else 
    {
        this->setWindowTitle(tr("Acharya-v%2.%3 (build %4)").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(BUILD));
    }
}

int SaveISCII(TextEdit* editor, QString filename);

bool ACIEdit::SaveDoc()
{
    TextEdit* editor = GetActiveEditor();
    if (!editor)
        return false;

    if (!editor->document()->isModified())
    {
        statusBar()->showMessage(tr("No changes need to be saved."), 2000);
        return true;
    }
    
    QString filename = editor->documentTitle();

    if (filename.isEmpty())
    {
        filename = QFileDialog::getSaveFileName(this, tr("Save File..."), lastSaveDir_, tr("ISCII Files (*.aci)"));
        if (filename.isEmpty())
            return false;
    }
    
    //append ".aci" if it is not there
    if (!filename.endsWith(".aci", Qt::CaseInsensitive))
        filename.append(".aci");
    
    /* backup the old file if it exists */
    if (QFile::exists(filename))
        QFile::copy(filename, filename + QString(".bak"));
        
    if (SaveISCII(editor, filename) == 0)
    {
        editor->setDocumentTitle(filename);
        editor->document()->setModified(false);
        setWindowModified(false);
        QFileInfo info(filename);
        lastSaveDir_ = info.absolutePath();
        tab_->setTabText(tab_->currentIndex(), info.fileName());
        UpdateTitleBar();
        statusBar()->showMessage(tr("Saved."), 2000);
        return true;
    }
    return false;
}

bool ACIEdit::SaveDocAs()
{
    TextEdit* editor = GetActiveEditor();
    if (!editor) return false;
    
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File..."), lastSaveDir_, tr("ISCII Files (*.aci)"));
    if (filename.isEmpty())
        return false;
        
    QFileInfo info(filename);
    lastSaveDir_ = info.absolutePath();
    
    //append ".aci" if it is not there
    if (!filename.endsWith(".aci", Qt::CaseInsensitive))
        filename.append(".aci");

    /* backup the old file if it exists */
    if (QFile::exists(filename))
        QFile::copy(filename, filename + QString(".bak"));
    
    if (SaveISCII(editor, filename) == 0)
    {
        editor->setDocumentTitle(filename);
        editor->document()->setModified(false);
        setWindowModified(false);
        tab_->setTabText(tab_->currentIndex(), info.fileName());
        UpdateTitleBar();
        statusBar()->showMessage(tr("Saved."), 2000);
        return true;
    }
    return false;
}

bool ACIEdit::CloseDoc()
{
    if (tab_->count() == 0)
        return true;

    TextEdit* editor = GetActiveEditor();
    bool close = true;
    
    if (!editor)
    {
        /* help text */
        QWidget* w = tab_->currentWidget();
        if (w)
        {
            tab_->removeTab(tab_->currentIndex());
            QTimer::singleShot(0, w, SLOT(deleteLater()));
            tab_->cornerWidget(Qt::TopRightCorner)->setEnabled(tab_->count() > 0);
            UpdateTitleBar();
        }
        return close;
    }
    
    if (editor->document()->isModified())
    { 
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Peixes:"),
                                   tr("The document has unsaved changes.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
             close = SaveDoc();
        else if (ret == QMessageBox::Cancel)
            close = false;
        else 
            close = true;
    }

    if (close)
    {
        tab_->removeTab(tab_->currentIndex());    
        QTimer::singleShot(0, editor, SLOT(deleteLater()));
        tab_->cornerWidget(Qt::TopRightCorner)->setEnabled(tab_->count() > 0);
        UpdateTitleBar();
    }
    return close;
}
/* commented out
void ACIEdit::PreviewDoc()
{
    TextEdit* editor = GetActiveEditor();
    if (editor)
    {
        QTextDocument* doc = editor->document();
        PrintPreview* preview = new PrintPreview(doc, this);
        preview->setWindowModality(Qt::WindowModal);
        preview->setAttribute(Qt::WA_DeleteOnClose);
        preview->show();
    }
}

void ACIEdit::PrintDoc()
{
    TextEdit* editor = GetActiveEditor();
    if (editor)
    {
        QTextDocument* doc = editor->document();
        if (!doc) return;

        QPrinter printer;
        QPrintDialog* dlg = new QPrintDialog(&printer, this);
        dlg->setWindowTitle(tr("Print Document..."));
        if (dlg->exec() != QDialog::Accepted) return;

        doc->print(&printer);
        statusBar()->showMessage(tr("Ready."));
    }
}
*/
/* defined in unicode.cpp */
int SaveUTF8(TextEdit* editor, QString filename);

void ACIEdit::SaveAsUtf8()
{
    TextEdit* editor = GetActiveEditor();
    if (editor)
    {
        QString filename = QFileDialog::getSaveFileName(this, "Save Unicode text output to...", lastSaveDir_, "Unicode(UTF8) Text Files (*.txt)");
        if (!filename.isEmpty())
        {
            //append ".aci" if it is not there
            if (!filename.endsWith(".txt", Qt::CaseInsensitive))
                filename.append(".txt");

            if (SaveUTF8(editor, filename) == 0)
            {
                lastSaveDir_ = QFileInfo(filename).absolutePath();
                statusBar()->showMessage(tr("Unicode(UTF8) text saved."), 2000);
            }
            else
            {
                statusBar()->showMessage(tr("Not saved."), 2000);
            }
        }
    }
}

void ACIEdit::SaveAsHTML()
{
    TextEdit* editor = GetActiveEditor();
    if (editor)
    {
        QString filename = QFileDialog::getSaveFileName(this, "Save HTML output to...", lastSaveDir_, "HTML Files (*.html)");
        if (!filename.isEmpty())
        {
            lastSaveDir_ = QFileInfo(filename).absolutePath();
            QFile file(filename);
            file.open(QIODevice::WriteOnly);
            QTextStream out(&file);
            out << editor->toHtml() << endl;
            statusBar()->showMessage(tr("HTML document saved."), 2000);
        }
    }
}
/* bang
void ACIEdit::SaveAsPDF()
{
    TextEdit* editor = GetActiveEditor();
    if (editor)
    {
        QTextDocument* doc = editor->document();
        QString filename = QFileDialog::getSaveFileName(this, "Save PDF output to...", lastSaveDir_, "PDF Files (*.pdf)");
        if (!filename.isEmpty())
        {
            lastSaveDir_ = QFileInfo(filename).absolutePath();
            QPrinter printer;
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(filename);
            doc->print(&printer);
            statusBar()->showMessage(tr("PDF document saved."), 2000);
        }
    }
}
*/
void ACIEdit::SaveAsRTF()
{
    TextEdit* editor = GetActiveEditor();
    if (editor)
    {
        QString filename = QFileDialog::getSaveFileName(this, "Save RTF output to...", lastSaveDir_, "RichText Files (*.rtf)");
        if (!filename.isEmpty())
        {
            lastSaveDir_ = QFileInfo(filename).absolutePath();
            editor->ToRTF(filename);
        }
    }
}

void ACIEdit::About()
{
    QMessageBox::about(this, tr("About ACIEdit..."),
                       tr("A simple text editor for Indic scripts. Although the files are saved using the <b>ISCII</b> encoding, this editor uses the <b>IMLI</b> encoding internally. More information can be found on the <a href=\"http://imli.sf.net\">project page</a>."));
}

void ACIEdit::Usage()
{
    QFile help_file(QDir::currentPath() + "/usage.html");
    if (!help_file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("Warning:"), "Unable to read help file.");
        return;
    }
    QTextStream in(&help_file);
    QTextEdit* help_viewer = new QTextEdit();
    help_viewer->setReadOnly(true);
    help_viewer->setHtml(in.readAll());
    tab_->addTab(help_viewer, help_viewer->documentTitle());
    tab_->setCurrentIndex(tab_->count() - 1);
    UpdateTitleBar();
}

void ACIEdit::CreateActions()
{
    newAct_ = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct_->setShortcut(tr("Ctrl+N"));
    newAct_->setStatusTip(tr("Create a new file"));
    connect(newAct_, SIGNAL(triggered()), this, SLOT(NewDoc()));

    openAct_ = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct_->setShortcut(tr("Ctrl+O"));
    openAct_->setStatusTip(tr("Open an existing file"));
    connect(openAct_, SIGNAL(triggered()), this, SLOT(OpenDoc()));

    clearHistoryAct_ = new QAction(tr("Clear History"), this);
    clearHistoryAct_->setEnabled(false);
    connect(clearHistoryAct_, SIGNAL(triggered()), this, SLOT(ClearFileHistory()));
    
    closeAct_ = new QAction(QIcon(":/images/closetab.png"), tr("&Close"), this);
    closeAct_->setShortcut(tr("Ctrl+W"));
    closeAct_->setStatusTip(tr("Close document"));
    connect(closeAct_, SIGNAL(triggered()), this, SLOT(CloseDoc()));

    saveAct_ = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct_->setShortcut(tr("Ctrl+S"));
    saveAct_->setStatusTip(tr("Save the document to disk"));
    connect(saveAct_, SIGNAL(triggered()), this, SLOT(SaveDoc()));

    saveAsAct_ = new QAction(tr("Save &As..."), this);
    saveAsAct_->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct_, SIGNAL(triggered()), this, SLOT(SaveDocAs()));

    saveAsUnicode_ = new QAction(tr("Save Unicode Text As..."), this);
    saveAsUnicode_->setStatusTip(tr("Save the document in Unicode (UTF8) format"));
    connect(saveAsUnicode_, SIGNAL(triggered()), this, SLOT(SaveAsUtf8()));

    saveAsHTML_ = new QAction(tr("Save HTML As..."), this);
    saveAsHTML_->setStatusTip(tr("Save the document in HTML format"));
    connect(saveAsHTML_, SIGNAL(triggered()), this, SLOT(SaveAsHTML()));

    /*
    saveAsPDF_ = new QAction(QIcon(":/images/pdf-document.png"), tr("Save PDF As..."), this);
    saveAsPDF_->setStatusTip(tr("Save the document in Portable Document Format"));
    connect(saveAsPDF_, SIGNAL(triggered()), this, SLOT(SaveAsPDF()));
*/
    
    saveAsRTF_ = new QAction(QIcon(":/images/rtf-document.png"), tr("Save RTF As..."), this);
    saveAsRTF_->setStatusTip(tr("Save the document in RichText Format"));
    connect(saveAsRTF_, SIGNAL(triggered()), this, SLOT(SaveAsRTF()));

    /*
    previewAct_ = new QAction(QIcon(":/images/preview.png"), tr("Print Preview..."), this);
    previewAct_->setStatusTip(tr("Preview the document as it would appear on paper..."));
    connect(previewAct_, SIGNAL(triggered()), this, SLOT(PreviewDoc()));
*/
    /*
    printAct_ = new QAction(QIcon(":/images/print.png"), tr("Print..."), this);
    printAct_->setShortcut(tr("Ctrl+P"));
    printAct_->setStatusTip(tr("Print the document..."));
    connect(printAct_, SIGNAL(triggered()), this, SLOT(PrintDoc()));
*/

    exitAct_ = new QAction(tr("E&xit"), this);
    exitAct_->setShortcut(tr("Ctrl+Q"));
    exitAct_->setStatusTip(tr("Exit the application"));
    connect(exitAct_, SIGNAL(triggered()), this, SLOT(close()));

    undoAct_ = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
    undoAct_->setShortcut(tr("Ctrl+Z"));
    undoAct_->setStatusTip(tr("Undo the last edit"));
    connect(undoAct_, SIGNAL(triggered()), this, SLOT(Undo()));

    cutAct_ = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct_->setShortcut(tr("Ctrl+X"));
    cutAct_->setStatusTip(tr("Cut the current selection's contents to the "
                             "clipboard"));
    connect(cutAct_, SIGNAL(triggered()), this, SLOT(Cut()));

    copyAct_ = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct_->setShortcut(tr("Ctrl+C"));
    copyAct_->setStatusTip(tr("Copy the current selection's contents to the "
                              "clipboard"));
    connect(copyAct_, SIGNAL(triggered()), this, SLOT(Copy()));

    pasteAct_ = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct_->setShortcut(tr("Ctrl+V"));
    pasteAct_->setStatusTip(tr("Paste the clipboard's contents at the current position."));
    connect(pasteAct_, SIGNAL(triggered()), this, SLOT(Paste()));

    pasteASCIIAct_ = new QAction(tr("Paste As ASCII"), this);
    pasteASCIIAct_->setShortcut(tr("Ctrl+Shift+V"));
    pasteASCIIAct_->setStatusTip(tr("Paste the clipboard's contents as ASCII."));
    connect(pasteASCIIAct_, SIGNAL(triggered()), this, SLOT(PasteASCII()));

    clearAct_ = new QAction(QIcon(":/images/clear.png"), tr("C&lear"), this);
    clearAct_->setStatusTip(tr("Delete the current selection"));
    connect(clearAct_, SIGNAL(triggered()), this, SLOT(Clear()));

    selectAllAct_ = new QAction(tr("Select All"), this);
    selectAllAct_->setShortcut(tr("Ctrl+A"));
    selectAllAct_->setStatusTip(tr("Select the entire text for subsequent Cut/Copy/Clear operation."));
    
    connect(selectAllAct_, SIGNAL(triggered()), this, SLOT(SelectAll()));

    toggleEnglish_ = new QAction(tr("&Toggle English"), this);
    toggleEnglish_->setShortcut(tr("F9"));
    toggleEnglish_->setStatusTip(tr("Switch between English and the last selected Indic script"));
    connect(toggleEnglish_, SIGNAL(triggered()), this, SLOT(ToggleEnglish()));

    findAct_ = new QAction(tr("Find..."), this);
    findAct_->setShortcut(QKeySequence::Find);
    
    findNextAct_ = new QAction(tr("Find &Next"), this);
    findNextAct_->setShortcut(QKeySequence::FindNext);
    connect(findNextAct_, SIGNAL(triggered()), this, SLOT(FindNext()));

    findPrevAct_ = new QAction(tr("Find &Previous"), this);
    findPrevAct_->setShortcut(QKeySequence::FindPrevious);
    connect(findPrevAct_, SIGNAL(triggered()), this, SLOT(FindPrev()));

    showKeyboardAct_ = new QAction(tr("Show Keyboard"), this);
    showKeyboardAct_->setCheckable(true);
    
    zoomInAct_ = new QAction(tr("Increase Text Size"), this);
    zoomInAct_->setShortcut(tr("Ctrl++"));
    connect(zoomInAct_, SIGNAL(triggered()), this, SLOT(ZoomIn()));
    
    zoomOutAct_ = new QAction(tr("Decrease Text Size"), this);
    zoomOutAct_->setShortcut(tr("Ctrl+-"));
    connect(zoomOutAct_, SIGNAL(triggered()), this, SLOT(ZoomOut()));

    showStatsAct_ = new QAction(tr("Show Statistics"), this);
    showStatsAct_->setCheckable(true);

    showCompletion_ =  new QAction(tr("Show Completions"), this);
    showCompletion_->setShortcut(tr("Ctrl+Space"));
    connect(showCompletion_, SIGNAL(triggered()), this, SLOT(ShowCompletions()));
    
    loadDictionary_ =  new QAction(tr("Load..."), this);
    loadDictionary_->setEnabled(false);
    connect(loadDictionary_, SIGNAL(triggered()), this, SLOT(LoadDictionary()));

    clearDictionary_ =  new QAction(tr("Clear"), this);
    //clearDictionary_->setEnabled(false);
    connect(clearDictionary_, SIGNAL(triggered()), this, SLOT(ClearDictionary()));
    
    indexDocument_ =  new QAction(tr("Index Document"), this);
    indexDocument_->setShortcut(tr("Ctrl+F8"));
    connect(indexDocument_, SIGNAL(triggered()), this, SLOT(IndexCurrentDocument()));
    
    aboutAct_ = new QAction(QIcon(":/images/about.png"), tr("&About"), this);
    aboutAct_->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct_, SIGNAL(triggered()), this, SLOT(About()));

    aboutQtAct_ = new QAction(QIcon(":/images/qtlogo.png"), tr("About &Qt"), this);
    aboutQtAct_->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct_, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    usageAct_ = new QAction(QIcon(":/images/help.png"), tr("&Usage"), this);
    usageAct_->setStatusTip(tr("Display usage information"));
    connect(usageAct_, SIGNAL(triggered()), this, SLOT(Usage()));

    cutAct_->setEnabled(true);
    copyAct_->setEnabled(true);
    // connect(textEdit, SIGNAL(copyAvailable(bool)),
    //             cutAct_, SLOT(setEnabled(bool)));
    //     connect(textEdit, SIGNAL(copyAvailable(bool)),
    //             copyAct_, SLOT(setEnabled(bool)));

    cmbScripts_ = new QComboBox(this);
    cmbScripts_->setEditable(false);

    cmbInputMethods_ = new QComboBox(this);
    cmbInputMethods_->setEditable(false);
    cmbInputMethods_->addItem(QString("Inscript"), QVariant(int(INSCRIPT)));
    cmbInputMethods_->addItem(QString("Phonetic"), QVariant(int(PHONETIC)));
}


void ACIEdit::CreateMenus()
{
    fileMenu_ = menuBar()->addMenu(tr("&File"));
    fileMenu_->addAction(newAct_);
    fileMenu_->addAction(openAct_);
    
    recentFilesMenu_ = fileMenu_->addMenu("Recent Files");
    recentFilesMenu_->addAction(clearHistoryAct_);
    recentFilesMenu_->addSeparator();

    fileMenu_->addAction(closeAct_);
    fileMenu_->addAction(saveAct_);
    fileMenu_->addAction(saveAsAct_);
    fileMenu_->addSeparator();
    fileMenu_->addAction(saveAsUnicode_);
    fileMenu_->addAction(saveAsHTML_);
    //fileMenu_->addAction(saveAsPDF_);
    fileMenu_->addAction(saveAsRTF_);
    fileMenu_->addSeparator();
    //fileMenu_->addAction(previewAct_);
    //fileMenu_->addAction(printAct_);
    fileMenu_->addSeparator();
    fileMenu_->addAction(exitAct_);

    editMenu_ = menuBar()->addMenu(tr("&Edit"));
    editMenu_->addAction(undoAct_);
    editMenu_->addAction(cutAct_);
    editMenu_->addAction(copyAct_);
    editMenu_->addAction(pasteAct_);
    editMenu_->addAction(pasteASCIIAct_);
    editMenu_->addAction(clearAct_);
    editMenu_->addSeparator();
    editMenu_->addAction(selectAllAct_);
    editMenu_->addSeparator();
    editMenu_->addAction(toggleEnglish_);

    searchMenu_ = menuBar()->addMenu(tr("Search"));
    searchMenu_->addAction(findAct_);
    searchMenu_->addAction(findNextAct_);
    searchMenu_->addAction(findPrevAct_);

    viewMenu_ = menuBar()->addMenu(tr("&View"));
    viewMenu_->addAction(showKeyboardAct_);
    viewMenu_->addAction(zoomInAct_);
    viewMenu_->addAction(zoomOutAct_);
    QMenu* styleMenu = viewMenu_->addMenu("UI Themes");
    QStringList styles = QStyleFactory::keys();
    styles.sort();
    uiStyleGroup_ = new QActionGroup(this);
    uiStyleGroup_->setExclusive(true);
    foreach (QString style, styles)
    {
        QAction* a = new QAction(style, this);
        a->setCheckable(true);
        connect(a, SIGNAL(triggered()), this, SLOT(SelectStyle()));
        uiStyleGroup_->addAction(a);
        styleMenu->addAction(a);
    }
    
    viewMenu_->addSeparator();
    viewMenu_->addAction(showStatsAct_);

    toolsMenu_ = menuBar()->addMenu(tr("&Tools"));
    QMenu* dictMenu = toolsMenu_->addMenu(tr("Dictionary"));
    dictMenu->addAction(loadDictionary_);
    dictMenu->addAction(clearDictionary_);
    dictMenu->addAction(indexDocument_);
    toolsMenu_->addAction(showCompletion_);
    
    menuBar()->addSeparator();

    helpMenu_ = menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(usageAct_);
    helpMenu_->addSeparator();
    helpMenu_->addAction(aboutAct_);
    helpMenu_->addAction(aboutQtAct_);
}


void ACIEdit::CreateToolBars()
{
/*
    fileToolBar_ = addToolBar(tr("File"));
    fileToolBar_->addAction(newAct_);
    fileToolBar_->addAction(openAct_);
    fileToolBar_->addAction(saveAct_);

    QToolBar* exportToolBar = addToolBar(tr("Export"));
    exportToolBar->addAction(saveAsHTML_);
    //exportToolBar->addAction(saveAsPDF_);
    exportToolBar->addAction(saveAsRTF_);

    //QToolBar* printToolBar = addToolBar(tr("Print"));
    //printToolBar->addAction(previewAct_);
    //printToolBar->addAction(printAct_);

    editToolBar_ = addToolBar(tr("Edit"));
    editToolBar_->addAction(cutAct_);
    editToolBar_->addAction(copyAct_);
    editToolBar_->addAction(pasteAct_);*/

    imliToolBar_ = new QToolBar(this);
    addToolBar(imliToolBar_);
    
    imliToolBar_->addWidget(cmbScripts_);
    imliToolBar_->addWidget(cmbInputMethods_);
}

void ACIEdit::CreateStatusBar()
{
    QStatusBar* status_bar = statusBar();
    status_bar->showMessage(tr("Ready."));
    statLabel_ = new QLabel(status_bar);
    statLabel_->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    statLabel_->setVisible(false);
    connect(showStatsAct_, SIGNAL(triggered(bool)), statLabel_, SLOT(setVisible(bool)));
    status_bar->addPermanentWidget(statLabel_);
    status_bar->addPermanentWidget(errorLabel_ = new QLabel(status_bar));
    errorLabel_->setText("...");
}

void ACIEdit::ReadSettings() //TODO font settings got here
{
    /* Parse out the section like this:
       [Fonts]
       <script>=<fontname>@<fontsize>
       ...
    */
    char* imli_dir = imli_home_dir(context_);
    QString fname = QString("%1/fontmap.ini").arg(imli_dir);
    QSettings font_settings(fname, QSettings::IniFormat);
    font_settings.beginGroup("Fonts");
    QStringList fontmap = font_settings.childKeys();
    foreach (QString key, fontmap)
    {
        QString rhs = font_settings.value(key).toString();
        if (rhs.isEmpty())
        {
            qDebug() << "Empty entry for script: %s" << key;
        }
        else
        {
            imli_script_t script = SCRIPT_UNSUPPORTED;
            if (key == "English")
                script = SCRIPT_ASCII;
            else
                script = imli_script_code(key.toAscii());

            if (script == SCRIPT_UNSUPPORTED)
            {
                qDebug() << "Script %s is not supported" << key;
            }
            else
            {
                QStringList tuple = rhs.split('@');
                if (tuple.empty())
                {
                    qDebug() << "Malformed line : %s" << rhs;
                }
                else
                {
                    bool ok;
                    QTextCharFormat format;
                    format.setFontFamily(tuple.at(0));
                    format.setFontPointSize(tuple.at(1).toInt(&ok));
		    styles_[int(script)] =  format;
                    cmbScripts_->addItem(key, QVariant(int(script)));
                }
            }
        }
    }
    font_settings.endGroup();

    /* other per-user settings */
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "UELTV", "ACIEdit");

    lastOpenDir_ = settings.value("LastOpenDir").toString();
    lastSaveDir_ = settings.value("LastSaveDir").toString();
    QString data = settings.value("ActiveScript").toString();
    int index = cmbScripts_->findText((data.isEmpty() ? QString("Hindi") : data));
    cmbScripts_->setCurrentIndex(index);
    data = settings.value("ActiveInputMethod").toString();
    index = cmbInputMethods_->findText((data.isEmpty() ? QString("Inscript") : data));
    cmbInputMethods_->setCurrentIndex(index);
    if (settings.contains("LastIndicScript"))
	lastScript_ = imli_script_t(settings.value("LastIndicScript").toInt());

    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
    
    /* virtual keyboard */
    bool showKeyboard = settings.value("Showkeyboard", true).toBool();
    showKeyboardAct_->setChecked(showKeyboard);
    virtualKeyboard_->setVisible(showKeyboard);

    /* UI Scheme */
    QString UIScheme = settings.value("UIScheme").toString();
    if (!UIScheme.isEmpty())
    {
        QApplication::setStyle(UIScheme);
        QList<QAction*> lst = uiStyleGroup_->actions();
        foreach (QAction* a, lst)
        {
            if (a->text() == UIScheme)
            {
                a->setChecked(true);
                break;
            }
        }
    }
    
    /* Recent files list - should be read in reverse to maintain MRU order :) */
    int nfiles = settings.beginReadArray("RecentFiles");
    for(int i = nfiles - 1; i >= 0; i--)
    {
        settings.setArrayIndex(i);
        QString filename = settings.value("file").toString();
        AddToFileHistory(filename);
    }
    
    settings.endArray();
}

void ACIEdit::WriteSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "UELTV", "ACIEdit");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("ShowKeyboard", showKeyboardAct_->isChecked());
    settings.setValue("ActiveScript", cmbScripts_->currentText());
    settings.setValue("ActiveInputMethod", cmbInputMethods_->currentText());
    settings.setValue("LastOpenDir", lastOpenDir_);
    settings.setValue("LastSaveDir", lastSaveDir_);
    settings.setValue("LastIndicScript", int(lastScript_));
    QAction* checkedUIStyle = uiStyleGroup_->checkedAction();
    if (checkedUIStyle)
        settings.setValue("UIScheme", checkedUIStyle->text());

    settings.beginWriteArray("RecentFiles");
    QList<QAction*> lst = recentFilesMenu_->actions();
    for(int i = 2; i < lst.size(); i++)
    {
        settings.setArrayIndex(i - 2);
        settings.setValue("file", lst.at(i)->text());
    }
    settings.endArray();
}

void ACIEdit::ScriptChanged(int index)
{
    imli_script_t script = imli_script_t(cmbScripts_->itemData(index).toInt());
    imli_select_script(context_, script);

    if  (script != SCRIPT_ASCII)
    {
        imli_keyboard_t im = 
            imli_keyboard_t(cmbInputMethods_->itemData(cmbInputMethods_->currentIndex()).toInt());
        imli_select_input_method(context_, im);
        lastScript_ = script;
        cmbInputMethods_->setEnabled(true);
        // for transliteration
        TextEdit* editor = GetActiveEditor();
        if (editor)
            editor->Transliterate(script);
    }
    else
    {
        cmbInputMethods_->setEnabled(false);
    }

    if (virtualKeyboard_->isVisible())
        virtualKeyboard_->Update();
}

void ACIEdit::InputMethodChanged(int index)
{
    imli_keyboard_t im = 
        imli_keyboard_t(cmbInputMethods_->itemData(index).toInt());
    imli_select_input_method(context_, im);

    if (virtualKeyboard_->isVisible())
        virtualKeyboard_->Update();
}

void ACIEdit::ToggleEnglish()
{
    int index = -1;
    
    if (imli_get_active_script(context_) == SCRIPT_ASCII)
    {
	index = cmbScripts_->findData(QVariant(int(lastScript_)));
        
	if (index == -1)
	    index = cmbScripts_->findText("Hindi");
    }
    else
    {
	index = cmbScripts_->findText("English");
    }
    cmbScripts_->setCurrentIndex(index);
}

void ACIEdit::Undo()
{
    TextEdit* editor = dynamic_cast<TextEdit*>(QApplication::focusWidget());
    if (!editor)
        editor = GetActiveEditor();

    if (editor) editor->Undo();
}

void ACIEdit::Cut()
{
    TextEdit* editor = dynamic_cast<TextEdit*>(QApplication::focusWidget());
    if (!editor)
        editor = GetActiveEditor();

    if (editor) editor->Cut();
}

void ACIEdit::Copy()
{
   TextEdit* editor = dynamic_cast<TextEdit*>(QApplication::focusWidget());
    if (!editor)
        editor = GetActiveEditor();

    if (editor) editor->Copy();
}

void ACIEdit::Paste()
{
   TextEdit* editor = dynamic_cast<TextEdit*>(QApplication::focusWidget());
    if (!editor)
        editor = GetActiveEditor();

    if (editor) editor->Paste();
}

void ACIEdit::PasteASCII()
{
   TextEdit* editor = dynamic_cast<TextEdit*>(QApplication::focusWidget());
    if (!editor)
        editor = GetActiveEditor();

    if (editor) editor->PasteAsASCII();
}

void ACIEdit::Clear()
{
   TextEdit* editor = dynamic_cast<TextEdit*>(QApplication::focusWidget());
    if (!editor)
        editor = GetActiveEditor();

    if (editor) editor->Clear();
}

void ACIEdit::SelectAll()
{
   TextEdit* editor = dynamic_cast<TextEdit*>(QApplication::focusWidget());
    if (!editor)
        editor = GetActiveEditor();

    if (editor) editor->SelectAll();
}

void ACIEdit::ReadPendingDatagrams()
{
    QUdpSocket* sock = dynamic_cast<QUdpSocket*>(sender());
    if (sock)
    {
        while (sock->hasPendingDatagrams())
        {
            QByteArray dgram;
            dgram.resize(sock->pendingDatagramSize());
            // we don't care where the datagram came from
            sock->readDatagram(dgram.data(), dgram.size());
            ProcessDatagram(dgram);
        }
    }
}

void ACIEdit::ProcessDatagram(QByteArray& dgram)
{
    QString msg(dgram);
    QRegExp exp("OPEN (.*)$");
    if (msg.contains(exp))
    {
        QString filename = exp.cap(1);
        OpenDoc(filename);
    }
}

void ACIEdit::PopupEditMenu(QContextMenuEvent *event)
{
    editMenu_->exec(event->globalPos());
}

void ACIEdit::DocumentWasModified()
{
    QTextDocument* doc = static_cast<QTextDocument*>(sender());
    setWindowModified(doc->isModified());
}

void ACIEdit::ZoomIn()
{
    TextEdit* editor = GetActiveEditor();
    if (editor)
        editor->ZoomIn(2);
}

void ACIEdit::ZoomOut()
{
    TextEdit* editor = GetActiveEditor();
    if (editor)
        editor->ZoomOut(2);
}

void ACIEdit::SelectStyle()
{
    QString name = ((QAction*)sender())->text();
    QApplication::setStyle(name);
}

/*
  \brief Helper to check if the \a filename is already open in a tab.
  \returns the \e index of the tab that corresponds to filename if \a
  filename is already open or -1.
*/
int ACIEdit::CheckTabs(QString filename)
{ 
    int ret = -1; 
    for(int i = 0; i < tab_->count(); i++) 
    { 
        QTextEdit* edit = dynamic_cast<TextEdit*>(tab_->widget(i)); 
        if (edit && edit->documentTitle() == filename)
        { 
            ret = i;
            break;
        }
    }
    return ret;
}

void ACIEdit::FlagBadState(QString msg)
{
    errorLabel_->setText(QString("<p fgcolor=\"#CC0000\" bgcolor=\"#000000\">%1</p>").arg(msg));
    QTimer::singleShot(2000, errorLabel_, SLOT(clear()));
}

/*!
  \brief Clear only the list of files from the menu leaving behind the Clear History
  menuitem and its separator.
*/
void ACIEdit::ClearFileHistory()
{
    QList<QAction*> lst = recentFilesMenu_->actions();
    for (int i = 2; i < lst.size(); i++)
    {
        QAction* a = lst.at(i);
        if (a == clearHistoryAct_)
            continue;
        recentFilesMenu_->removeAction(a);
    }
    clearHistoryAct_->setEnabled(false);
}

/*!  
  \brief Add \a filename to a fixed (currently 10) list of recently opened
  files. Entries are evicted under the least recently used policy.
  The list appears under the File->Recent Files submenu.
*/
void ACIEdit::AddToFileHistory(QString filename)
{
    QList<QAction*> lst = recentFilesMenu_->actions();
    int size = lst.size();

    /* check if it present in the list, starting from 1 because item 0
     * is "Clear History" which should not be removed.
     */
    for (int i = 1; i < size; i++)
    {
        if (lst.at(i)->text() == filename)
            return;
    }

    QAction* a = new QAction(filename, recentFilesMenu_);
    connect(a, SIGNAL(triggered()), this, SLOT(OpenDoc()));

    if (size == 2)
    {
        /* append only if the file list is empty */
        recentFilesMenu_->addAction(a);
    }
    else
    {
        QAction* first = lst.at(2);
        QAction* last  = lst.at(size-1);
        recentFilesMenu_->insertAction(first, a);
        if (size > 12)
            recentFilesMenu_->removeAction(last);
    }
    clearHistoryAct_->setEnabled(true);
}

/*!
 * \brief Locate the next instance of the string of glyphs typed in
 * the find word text box. Because of this glyph based search, it
 * looks only for matching words. FIXME: currently using glyph based
 * search. Should do a syllable based search and the functionality
 * should be moved to \a TextStorage.
 */
void ACIEdit::FindNext()
{
    if (!findFrame_->isVisible())
    {
        findFrame_->show();
        return;
    }
    
    TextEdit* editor = GetActiveEditor();
    if (editor)
    {
        bool result = 
            editor->find(txtFind_->toPlainText(), QTextDocument::FindWholeWords);
        if (result)
            editor->SyncCursors();
        else
            statusBar()->showMessage(tr("Search text not found."), 2000);
    }
}

/*!
 * \brief Locate the previous instance of the string of glyphs typed in
 * the find word text box. Because of this glyph based search, it
 * looks only for matching words. FIXME: currently using glyph based
 * search. Should do a syllable based search and the functionality
 * should be moved to \a TextStorage.
 */

void ACIEdit::FindPrev()
{
    if (!findFrame_->isVisible())
    {
        findFrame_->show();
        return;
    }
    
    TextEdit* editor = GetActiveEditor();
    if (editor)
    {
        QTextCursor current_cursor = editor->textCursor();
        if (current_cursor.hasSelection())
            current_cursor.movePosition(QTextCursor::WordLeft);
        
        QTextCursor c = editor->document()->find(txtFind_->toPlainText(),
                                                 current_cursor,
                                                 QTextDocument::FindBackward | 
                                                 QTextDocument::FindWholeWords);
            
        if (c.isNull())
        {
            statusBar()->showMessage(tr("Search text not found."), 2000);
        }
        else
        {
            editor->setTextCursor(c);
            editor->SyncCursors();
        }
    }
}

void ACIEdit::IndexCurrentDocument()
{
    TextEdit* editor = GetActiveEditor();
    if (editor)
	editor->AddSelectionToDictionary();
}

void ACIEdit::ShowCompletions()
{
    TextEdit* editor = GetActiveEditor();
    if (editor)
    {
	int n = editor->ShowCompletions();
	if (n == -1)
	    statusBar()->showMessage("Nothing to complete.", 2000);
	else if (n == 0)
	    statusBar()->showMessage("No completions.", 2000);
	else
	    statusBar()->showMessage(QString("%1 completions").arg(n),  2000);
    }
}

void ACIEdit::LoadDictionary()
{
    /* to be done */
}

void ACIEdit::ClearDictionary()
{
    Dictionary* dict = TextEdit::dict_;
    if (dict)
	dict->Clear();
}
