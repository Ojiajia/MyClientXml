#include <QObject>
#include <QtWidgets>
#include <QtCore>
#include <QtNetwork>
#include "myclientxml.h"
#include <QApplication>
#include <QDialog>
#include <QInputDialog>
#include <QWidget>
#include <stdlib.h>
#include <QFlags>

MyClient::MyClient(const QString& strHost,
                   int            nPort,
                   QWidget*       pwgt /*=0*/,
                   QString        userNickName
                  ) : QWidget(pwgt)
                 , m_nNextBlockSize(0)

{
    setWindowTitle(userNickName);
    m_pTcpSocket = new QTcpSocket(this);
    m_pTcpSocket->connectToHost(strHost, nPort);
    NickName = userNickName;
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotSendNick()));
//    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));

    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,         SLOT(slotError(QAbstractSocket::SocketError))
           );

    m_ptxtInfo  = new QTextEdit; // многострочное поле
    m_ptxtInput = new QLineEdit; // однострочное поле
    m_UserList  = new QComboBox; // список ников

    m_ptxtInfo->setReadOnly(true);

    QPushButton* pcmd = new QPushButton("&Send");
    inputToText =  m_ptxtInput->text();
    connect(pcmd, SIGNAL(clicked()), SLOT(slotSendToServer()));
    connect(m_ptxtInput, SIGNAL(returnPressed()),this,SLOT(slotSendToServer()));

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(m_ptxtInfo);
    pvbxLayout->addWidget(m_ptxtInput);
    pvbxLayout->addWidget(m_UserList);
    pvbxLayout->addWidget(pcmd);
    setLayout(pvbxLayout);
    m_ptxtInput->setText("");

}

//***********************************************************************
// получение данных от сервера
void MyClient::slotReadyRead()
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_5_3);
    for (;;) {

        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }
        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        in >> str;
        m_ptxtInfo->append(time.toString()+" "+ str);
        m_nNextBlockSize = 0;
    }
    qDebug() << "slotReadyRead works";
}

//***********************************************************************

void MyClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    m_ptxtInfo->append(strError); // текст отображается в виджете
                                  // многострочного текстового поля
}

//***********************************************************************

// метод отсылки запроса клиента серверу

void MyClient::slotSendToServer()
{

    //send_msg(1,info);
    int intMsgType = 2;
    QString strMsgType = QString::number(intMsgType);
    QString str =  "<data><type>" + strMsgType + "</type><nickname>" + NickName +
            "</nickname><msg>" +  m_ptxtInput->text() + "</msg></data>";
    sendData(str);
    qDebug() << "msg from client: " << str;
}


//***********************************************************************

void MyClient::slotSendNick()
{
    //send_msg(2,info);
    //if (flag.testFlag(MyClient::nick))
    int intMsgType = 1;
    QString strMsgType = QString::number(intMsgType);
    QString str = "<data><type>" + strMsgType + "</type><nickname>" + NickName +
            "</nickname></data>";

    sendData(str);
    qDebug() << "nick from client: " << str;
}

//***********************************************************************

void MyClient::sendData(QString str)
{
    QByteArray  arrBlock; // массив байтов
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << quint16(0) << str ;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    m_pTcpSocket->write(arrBlock);
    m_ptxtInput->setText("");
}

//***********************************************************************

// клиент подсоединился к серверу

void MyClient::slotConnected()
{
    m_ptxtInfo->append("Received the connected() signal");
}

