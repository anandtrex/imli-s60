/*************************************************************
 * CentralWidget.h
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#ifndef __CENTRAL_WIDGET_H__
#define __CENTRAL_WIDGET_H__

#include <QWidget>

class ACIEdit;
class QToolButton;
class TextEdit;
class QTabWidget;
class QFrame;
class QCheckBox;
class VirtualKeyboard;

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    CentralWidget(ACIEdit* parent);
    ~CentralWidget();

    // tab for holding pages of documents
    QTabWidget* tabWidget;
    
    // frame for "find" dialog
    QFrame* findFrame;
    QToolButton* btnCloseFindFrame;
    TextEdit* txtFind;
    QToolButton* btnFindPrevious;
    QToolButton* btnFindNext;
    QCheckBox* chkWholeWords;
    VirtualKeyboard* virtualKeyboard;
    
protected:
    void SetupWidgets(ACIEdit* parent);
    void keyPressEvent(QKeyEvent* event);
    void paintEvent(QPaintEvent* event);
};

#endif
