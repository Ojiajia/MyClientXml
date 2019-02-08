#ifndef MYCLIENT_H
#define MYCLIENT_H
#pragma once
#include <QWidget>
#include <QTcpSocket>
#include <QDialog>
#include <QtWidgets>
#include <QInputDialog>
#include <QFlags>

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
    QString          str;
    void             sendData(QString str);

public:
    MyClient        (const QString& strHost,
                     int            nPort,
                     QWidget*       pwgt = 0,
                     QString        userNickName = "");
    QString          NickName;

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

class InputDialog : public QDialog {
Q_OBJECT

private:
//    QLineEdit* m_ptxtFirstName;

public:
    InputDialog     (QWidget* pwgt = 0);
    QLineEdit*      m_ptxtFirstName;
    QString         slotGiveNickName();
    QString         userNickName;
//private slots:
//    void slotGiveNickName();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MyClient::Options)
#endif // MYCLIENT_H
