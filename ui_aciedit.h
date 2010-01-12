/********************************************************************************
** Form generated from reading UI file 'aciedit.ui'
**
** Created: Wed Dec 23 00:53:41 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACIEDIT_H
#define UI_ACIEDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_acieditClass
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *acieditClass)
    {
        if (acieditClass->objectName().isEmpty())
            acieditClass->setObjectName(QString::fromUtf8("acieditClass"));
        acieditClass->resize(800, 600);
        centralwidget = new QWidget(acieditClass);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        acieditClass->setCentralWidget(centralwidget);
        menubar = new QMenuBar(acieditClass);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        acieditClass->setMenuBar(menubar);
        statusbar = new QStatusBar(acieditClass);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        acieditClass->setStatusBar(statusbar);

        retranslateUi(acieditClass);

        QMetaObject::connectSlotsByName(acieditClass);
    } // setupUi

    void retranslateUi(QMainWindow *acieditClass)
    {
        acieditClass->setWindowTitle(QApplication::translate("acieditClass", "MainWindow", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class acieditClass: public Ui_acieditClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACIEDIT_H
