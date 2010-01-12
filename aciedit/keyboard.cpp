/*************************************************************
 * keyboard.cpp
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#include "keyboard.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QTextCharFormat>
#include <QObject>
#include <QApplication>
#include <QCoreApplication>
#include <QKeyEvent>
#include "TextEdit.h"
#include "libimli.h"




VirtualKeyboard::VirtualKeyboard(imli_context_t ctx, const std::vector<QTextCharFormat>& styles, QWidget* parent)
    : QFrame(parent), context_(ctx), styles_(styles), shifted_(false)
{
	setupUi(this);
    //setMinimumSize(270, 120);
    //SetupWidgets();
	initCharNameMapping();
	SetupConnections();
	
	//To make sure the QPushButtons don't get focus
	const QObjectList& objects = this->verticalLayoutWidget->children();
	for (int i = 0; i < objects.size(); i++)
	{
		QPushButton* button = qobject_cast<QPushButton*>(objects.at(i));
		if (button)
		{
			button->setFocusPolicy(Qt::NoFocus);
		}
	}
    
}

VirtualKeyboard::~VirtualKeyboard()
{
}

void VirtualKeyboard::Clicked()
{
    QString key = getObjectName(sender()->objectName());
    Qt::KeyboardModifiers modifiers = Qt::NoModifier;
    QChar c;
    /* key for the synthetic event */
    int k;
    /* text for the synthetic event - used from the above QChar */
    QString text;
    
    if (key == "return")
    {
        k = Qt::Key_Return;
    }
    else if (key == "space")
    {
        k = Qt::Key_Space;
    }
    else if (key == "tab")
    {
        k = Qt::Key_Tab;
    }
    else if (key == "backspace")
    {
        k = Qt::Key_Backspace;
    }
    else 
    {
        if (shifted_)
        {
			c = key[1];
			modifiers = Qt::ShiftModifier;
        }
        else
        {
            c = key[0];
        }
        text = c;
        k = (int)c.unicode();
    }

    TextEdit* w = qobject_cast<TextEdit*>(QApplication::focusWidget());
    if (w)
    {
        QKeyEvent event(QEvent::KeyPress,
                        k,
                        modifiers,
                        text);
        QCoreApplication::sendEvent((QObject*)w, &event);
    }
}

void VirtualKeyboard::Update()
{
//TODO font set here
    imli_script_t script = imli_get_active_script(context_);
    if (script == SCRIPT_UNSUPPORTED) 
        return;
    
    QFont font = styles_[script].font();
    //font.setPointSize((script == SCRIPT_ASCII) ? 12 : 14);
    font.setPointSize(8);
    
    const QObjectList& objects = this->verticalLayoutWidget->children();

    int index = (shifted_ ? 1 : 0);
    
    for (int i = 0; i < objects.size(); i++)
    {
		
        QPushButton* child = dynamic_cast<QPushButton*>(objects.at(i));
        if (child)
        {
			child->setEnabled(true);
            QString name = getObjectName(child->objectName());
            if (name.length() > 2){
				continue;
            }
            
            child->setFont(font);
            char key = name.toAscii()[index];
            if (script != SCRIPT_ASCII)
            {
                syl_t syl = SYL_INVALID;
                int del;
                syl = imli_process_key(context_,
                                       key,
                                       SCRIPT_UNSUPPORTED,
                                       SYL_INVALID,
                                       FLAG_NONE,
                                       &del);
                if (syl != SYL_INVALID)
                {
                    QByteArray glyphs;
                    glyphs.reserve(32);
                    size_t len = 0;
                    
                    imli_status_t status = syl_to_glyphs(context_,
                                                         syl,
                                                         script,
                                                         (byte_t*)glyphs.data(), 
                                                         glyphs.capacity(),
                                                         &len);
                    if (status == IMLI_STATUS_OK)
                    {
                        glyphs.resize(len);
                        child->setText(QString::fromAscii(glyphs, glyphs.size()));
                    }
                    else
                    {
                        child->setText("");
                    }
                }
                else
                {
                    child->setText("");
                }
            }
            else
            {
                QString label(key);
                if (key == '&')
                    label.append('&');
                
                child->setText(label);
            }
            if(child->text().operator ==("")){
				child->setDisabled(true); //Disable empty buttons
            }
        }
    }
}

void VirtualKeyboard::setVisible(bool state)
{
    if (state)
        this->Update();
    
    QFrame::setVisible(state);
    //this->verticalLayoutWidget->setVisible(state);
}

void VirtualKeyboard::setShifted(bool state)
{
    shifted_ = state;
    if (this->isVisible())
        this->Update();
}

void VirtualKeyboard::SetupConnections()
{
    const QObjectList& objects = this->verticalLayoutWidget->children();
    for (int i = 0; i < objects.size(); i++)
    {
        QPushButton* button = qobject_cast<QPushButton*>(objects.at(i));
        if (button && button->objectName() != "caps" && button->objectName() != "shift")
        {
            connect(button, SIGNAL(clicked()), this, SLOT(Clicked()));
        }
    }
    connect(this->shift, SIGNAL(toggled(bool)), this, SLOT(setShifted(bool)));
}

void VirtualKeyboard::initCharNameMapping(){
	//Required if using QtDesigner to design the keyboard.
	//TODO instead of storing of characters in objectname store in something else?
	//This might be a memory hog?
	charNameMap = QHash<QString, QChar>();
	charNameMap.insert("tilda",'~');
	charNameMap.insert("grave",'`');
	charNameMap.insert("one",'1');
	charNameMap.insert("exclamation",'!');
	charNameMap.insert("two",'2');
	charNameMap.insert("at",'@');
	charNameMap.insert("three",'3');
	charNameMap.insert("hash",'#');
	charNameMap.insert("four",'4');
	charNameMap.insert("dollar",'$');
	charNameMap.insert("five",'5');
	charNameMap.insert("percentage",'%');
	charNameMap.insert("six",'6');
	charNameMap.insert("caret",'^');
	charNameMap.insert("seven",'7');
	charNameMap.insert("ampersand",'&');
	charNameMap.insert("eight",'8');
	charNameMap.insert("star",'*');
	charNameMap.insert("nine",'9');
	charNameMap.insert("openBrace",'(');
	charNameMap.insert("zero",'0');
	charNameMap.insert("closeBrace",')');
	charNameMap.insert("minus",'-');
	charNameMap.insert("underscore",'_');
	charNameMap.insert("equals",'=');
	charNameMap.insert("plus",'+');
	//openSquareBrace_openCurlyBrace
	charNameMap.insert("openSquareBrace",'[');
	charNameMap.insert("openCurlyBrace",'{');	
	charNameMap.insert("closeSquareBrace",']');
	charNameMap.insert("closeCurlyBrace",'}');
	//backslash_pipe
	charNameMap.insert("backslash",'\\');
	charNameMap.insert("pipe",'|');
	//semicolon_colon
	charNameMap.insert("semicolon",';');
	charNameMap.insert("colon",':');
	//singleQuote_doubleQuote
	charNameMap.insert("singleQuote",'\'');
	charNameMap.insert("doubleQuote",'\"');
	//comma_lessThan
	charNameMap.insert("comma",',');
	charNameMap.insert("lessThan",'<');
	//period_greaterThan
	charNameMap.insert("period",'.');
	charNameMap.insert("greaterThan",'>');
	//frontslash_questionMark
	charNameMap.insert("frontslash",'/');
	charNameMap.insert("questionMark",'?');	
}

QString VirtualKeyboard::getObjectName(QString objectName){
	QString realObjectName = QString(2,'a');
	QStringList listChars = objectName.split("_");
	if(listChars.size()==1)
		return objectName;
	realObjectName[0].operator =(charNameMap.value(listChars.value(0)));
	realObjectName[1].operator =(charNameMap.value(listChars.value(1)));
	return realObjectName;
}
