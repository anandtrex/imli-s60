/*************************************************************
 * CentralWidget.cpp
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#include "CentralWidget.h"
#include "aciedit.h"
#include "TextEdit.h"
#include "keyboard.h"

#include <QVBoxLayout>

CentralWidget::CentralWidget(ACIEdit* parent)
    : QWidget(parent)
{
    SetupWidgets(parent);
}

CentralWidget::~CentralWidget()
{
}

void CentralWidget::SetupWidgets(ACIEdit* parent)
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->setSpacing(5);
    vlayout->setMargin(0);
    
    virtualKeyboard = new VirtualKeyboard(parent->GetIMLIContext(), parent->GetStyles(), this);
    virtualKeyboard->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    virtualKeyboard->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    
    vlayout->addWidget(virtualKeyboard);
    
    tabWidget = new QTabWidget(this);
    vlayout->addWidget(tabWidget);
    
    findFrame = new QFrame(this);
    findFrame->setVisible(false);
    findFrame->setFrameShape(QFrame::StyledPanel);
    findFrame->setFrameShadow(QFrame::Raised);
    QHBoxLayout* hlayout = new QHBoxLayout(findFrame);
    hlayout->setSpacing(5);
    btnCloseFindFrame = new QToolButton(findFrame);
    btnCloseFindFrame->setIcon(QIcon(":/images/closetab.png"));
    btnCloseFindFrame->setAutoRaise(true);
    connect(btnCloseFindFrame, SIGNAL(clicked()), findFrame, SLOT(hide()));
    hlayout->addWidget(btnCloseFindFrame);
 
    hlayout->addWidget(new QLabel(tr("Find Word: ")));
    
    txtFind = new TextEdit(parent->GetIMLIContext(), parent->GetStyles(), 0);
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0),
                           static_cast<QSizePolicy::Policy>(0));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(txtFind->sizePolicy().hasHeightForWidth());
    txtFind->setSizePolicy(sizePolicy);
    txtFind->setMinimumSize(QSize(150, 32));
    hlayout->addWidget(txtFind);
    
    btnFindNext = new QToolButton(findFrame);
    btnFindNext->setIcon(QIcon(":/images/findnext.png"));
    btnFindNext->setText(tr("Next"));
    btnFindNext->setAutoRaise(true);
    connect(btnFindNext, SIGNAL(clicked()), parent, SLOT(FindNext()));
    hlayout->addWidget(btnFindNext);
    
    btnFindPrevious = new QToolButton(findFrame);
    btnFindPrevious->setIcon(QIcon(":/images/findprev.png"));
    btnFindPrevious->setText(tr("Previous"));
    btnFindPrevious->setAutoRaise(true);
    connect(btnFindPrevious, SIGNAL(clicked()), parent, SLOT(FindPrev()));
    hlayout->addWidget(btnFindPrevious);

    // chkWholeWords = new QCheckBox(findFrame);
//     chkWholeWords->setText(tr("Whole Words"));
//     hlayout->addWidget(chkWholeWords);
        
    hlayout->addItem(new QSpacerItem(81, 21, QSizePolicy::Expanding, QSizePolicy::Minimum));

    vlayout->addWidget(findFrame);
}

void CentralWidget::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    if (key == Qt::Key_Escape)
    {
        findFrame->hide();
        event->accept();
    }
    else event->ignore();
}

void CentralWidget::paintEvent(QPaintEvent* event)
{
	if(virtualKeyboard->isVisible()){
		virtualKeyboard->Update();
	}
	QWidget::paintEvent(event);
	//qDebug()<<"Paint event called";
	return;
}
