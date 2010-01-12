/*************************************************************
 * aciedit.h
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#ifndef __ACIEDIT_H__
#define __ACIEDIT_H__

#include <QtGui>

#include <libimli.h>

class TextEdit;
class VirtualKeyboard;

class ACIEdit : public QMainWindow
{
Q_OBJECT

public:
    ACIEdit(imli_context_t context, QWidget* parent = 0);
    ~ACIEdit();
    void PopupEditMenu(QContextMenuEvent *event);
    imli_context_t GetIMLIContext(){ return context_; }
    std::vector<QTextCharFormat>& GetStyles(){ return styles_; }
    
protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void NewDoc();
    TextEdit* NewTab();
    void OpenDoc();
    void OpenDoc(QString);
    bool SaveDoc();
    bool SaveDocAs();
    void SaveAsUtf8();
    void SaveAsHTML();
    //void SaveAsPDF();
    void SaveAsRTF();
    bool CloseDoc();
    //void PreviewDoc();
    //void PrintDoc();
    void About();
    void Usage();
    void UpdateTitleBar();
    void ScriptChanged(int);
    void InputMethodChanged(int);
    void ToggleEnglish();
    void Undo();
    void Cut();
    void Copy();
    void Paste();
    void PasteASCII();
    void Clear();
    void SelectAll();
    void DocumentWasModified();
    void ZoomIn();
    void ZoomOut();
    void SelectStyle();
    
    void ReadPendingDatagrams();
    void FlagBadState(QString);

    void ClearFileHistory();

    void FindNext();
    void FindPrev();
    
    void IndexCurrentDocument();
    void ShowCompletions();
    void LoadDictionary();
    void ClearDictionary();
    
private:
    TextEdit* GetActiveEditor();

    void InitContext();

    void CreateActions();
    void CreateMenus();
    void CreateToolBars();
    void CreateStatusBar();
    void ReadSettings();
    void WriteSettings();
    
    //resizeEvent() method get called when user change screen mode.
    //void resizeEvent(QResizeEvent*);
    
    void ProcessDatagram(QByteArray&);
    int  CheckTabs(QString);
    
    void AddToFileHistory(QString filename);
    
    VirtualKeyboard* virtualKeyboard_;

    QTabWidget* tab_;
    TextEdit* txtFind_;
    QCheckBox* chkWholeWords_;
    QFrame* findFrame_;
    
    QMenu *fileMenu_;
    QMenu *recentFilesMenu_;
    QMenu *editMenu_;
    QMenu *searchMenu_;
    QMenu *viewMenu_;
    QMenu *toolsMenu_;
    QMenu *helpMenu_;
    QToolBar *fileToolBar_;
    QToolBar *editToolBar_;
    QToolBar *imliToolBar_;
    QToolBar *searchToolBar_;
    QAction *newAct_;
    QAction *openAct_;
    QAction *clearHistoryAct_;
    QAction *closeAct_;
    QAction *saveAct_;
    QAction *saveAsAct_;
    //QAction *saveAsPDF_;
    QAction *saveAsRTF_;
    QAction *saveAsHTML_;
    QAction *saveAsUnicode_;
    //QAction *printAct_;
    //QAction *previewAct_;
    QAction *exitAct_;
    QAction *undoAct_;
    QAction *cutAct_;
    QAction *copyAct_;
    QAction *pasteAct_;
    QAction *pasteASCIIAct_;
    QAction *clearAct_;
    QAction *selectAllAct_;
    QAction *findAct_;
    QAction *findNextAct_;
    QAction *findPrevAct_;
    QAction *toggleEnglish_;
    QAction *showKeyboardAct_;
    QAction *zoomInAct_;
    QAction *zoomOutAct_;
    QAction *showStatsAct_;
    QActionGroup *uiStyleGroup_;
    QAction *showCompletion_;
    QAction *loadDictionary_;
    QAction *clearDictionary_;
    QAction *indexDocument_;
    QAction *aboutAct_;
    QAction *aboutQtAct_;
    QAction *usageAct_;
    QComboBox *cmbScripts_;
    QComboBox *cmbInputMethods_;
    QLabel    *statLabel_;
    QLabel    *errorLabel_;
    
    std::vector<QTextCharFormat> styles_;
    imli_context_t context_;
        
    QString lastOpenDir_;
    QString lastSaveDir_;
    int id_;
    imli_script_t lastScript_;
};

#endif
