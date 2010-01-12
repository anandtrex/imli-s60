/*************************************************************
 * main.cpp
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
#include <QApplication>
#include <QtNetwork>
#include "libimli.h"
#include "aciedit.h"

// This is a GCCE toolchain workaround needed when compiling with GCCE
// and using main() entry point
#ifdef __GCCE__
#include <staticlibinit_gcce.h>
#endif

static imli_context_t context;

void InitContext ()
{
    if (imli_context_new(&context, NULL) != IMLI_STATUS_OK)
    {
        QMessageBox::critical(0, QString("Fatal Error:"), QString("Failed to initialize IMLI context. Exiting..."));
        ::exit(42);
    }
    imli_select_script(context, SCRIPT_HINDI);
    
    if (init_iscii_tables(context) != IMLI_STATUS_OK)
    {
        QMessageBox::critical(0, QString("Fatal Error:"), QString("Failed to initialize ISCII context. Exiting..."));
        ::exit(42);
    }
    
}

void FreeContext ()
{
    imli_context_free(context);
}

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(aciedit);
    int retval = 0;
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/aciedit_32.png"));
    
    // see if another instance is running, if so forward the command
    // line arguments to it
    //FIXME: make the port no configurable
    QUdpSocket* sock = new QUdpSocket(&app);
    if (!sock->bind(QHostAddress::LocalHost, 12006))
    {
        QStringList args = app.arguments();
        for(int i = 1; i < args.size(); i++)
        {
            QString cmd = "OPEN " + args.at(i);
            sock->writeDatagram(cmd.toLatin1(), QHostAddress::LocalHost, 12006);
        }
    }
    else
    {
        InitContext();
        ACIEdit* main_window = new ACIEdit(context);
        main_window->setGeometry(0, 0, 640, 360);
        main_window->connect(sock, SIGNAL(readyRead()), main_window, SLOT(ReadPendingDatagrams()));
        main_window->show();
        QStringList args = app.arguments();
        for(int i = 1; i < args.size(); i++)
            main_window->OpenDoc(args.at(i));
        
        retval = app.exec();
        delete main_window;
        FreeContext();
    }
    return retval;
}
