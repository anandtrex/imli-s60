/*************************************************************
 * keyboard.h
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <libimli.h>

#include <QFrame>

#include "ui_keyboard.h"

class QTextCharFormat;

class VirtualKeyboard : public QFrame, private Ui::VirtualKeyboard
{
	Q_OBJECT

public:
    VirtualKeyboard(imli_context_t ctx, const std::vector<QTextCharFormat>&, QWidget* parent = 0);
    ~VirtualKeyboard();

public slots:
    void Update();
    void setVisible(bool);
    void setShifted(bool);

protected slots:
    void Clicked();

protected:
    void SetupWidgets();
    void initCharNameMapping();
    void SetupConnections();
    imli_context_t context_;
    const std::vector<QTextCharFormat>& styles_;
    bool shifted_;
    
private:
    QHash<QString, QChar> charNameMap;
    QString getObjectName(QString);
    
};

#endif //__KEYBOARD_H__
