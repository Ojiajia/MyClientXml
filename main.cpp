#include <QApplication>
#include <QtNetwork>
#include <QtWidgets>
#include "myclientxml.h"
#include <stdio.h>
#include <stdlib.h>
#include <QtXml>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);



    int nPort;
    QString userNickName;
    QString strHost;

    if (argc != 4) {
        //printf(argc);
        printf("Error: found %d arguments. Needs exactly 3", argc-1);
        exit(1);
    }
    nPort = atoi(argv[1]);
    strHost = argv[2];
    userNickName = argv[3];


//    const QString& nick = new const QString;
//    *userNickName = & nick;

    MyClient client(strHost, nPort, 0, userNickName);
        client.show();


        return app.exec();
    }
