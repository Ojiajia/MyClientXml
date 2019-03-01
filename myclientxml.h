#ifndef MYCLIENT_H
#define MYCLIENT_H
#pragma once
#include <QWidget>
#include <QTcpSocket>
#include <QDialog>
#include <QtWidgets>
#include <QInputDialog>
#include <QFlags>
#include <QtXml>

class QTextEdit;
class QLineEdit;

class MyClient : public QWidget{
Q_OBJECT

private:
    QTcpSocket*      m_pTcpSocket;     // управление клиентом
    QTextEdit*       m_ptxtInfo;       // отображение информации
    QLineEdit*       m_ptxtInput;      // ввод информации
    quint16          m_nNextBlockSize; // хранение  длины следующего полученного
                                       // от сокета блока
    QString*         Message;
    QString          inputToText;
    QComboBox*       m_UserList;
    QString          xmlMsg;


    void             sendData(QString str);

    QMap<QTcpSocket*,QString> usersList;

public:
    MyClient        (const QString& strHost,
                     int            nPort,
                     QWidget*       pwgt = 0,
                     QString        userNickName = "");
    QString          NickName;
    QString          traverseNode(QDomNode& domNode, QString &str2);
    QString          str2;
    QString          str;

    // флаги для сообщений
    enum Option
    {
        publicMsg  = 0x0,
        privateMsg = 0x1,
        nick       = 0x2
    };
    Q_DECLARE_FLAGS(Options, Option)


private slots:

// void - не возвращает никакого значения

    void slotReadyRead   (                            );
    void slotError       (QAbstractSocket::SocketError);
    void slotSendToServer(                            );
    void slotConnected   (                            );  
    void slotSendNick    (                            );
};
