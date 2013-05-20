#include "server.h"
#include "QTime"
#include "QDataStream"




Server::Server(int nPort, QObject *parent):QObject(parent)
{
    connected=false;
    reading=false;
    m_ptcpServer=new QTcpServer(this);

    if(!m_ptcpServer->listen(QHostAddress::Any,nPort))
    {
        qDebug()<<"Server error";
        m_ptcpServer->close();
        return;
    }
    else
        qDebug()<<"Server created";
    connect(m_ptcpServer,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));

//m_ptcpServer->listen(QHostAddress::Any,nPort);

}

Server::~Server()
{
}
void Server::sendToClient(QTcpSocket *pScoket, const QString &str)
{
    qDebug()<<"Send to client";
    QByteArray arrBlock;
    QDataStream out(&arrBlock,QIODevice::WriteOnly);
    out.setVersion((QDataStream::Qt_4_7));
    out<<qint64(0)<<QTime::currentTime()<<str;
    out.device()->seek(0);
    out<<qint64(arrBlock.size()-sizeof(qint64));
    pScoket->write(arrBlock);
}

void Server::sendToClient(QTcpSocket *pScoket)
{
    qDebug()<<"Send to client";
    QByteArray arrBlock;
    QDataStream out(&arrBlock,QIODevice::WriteOnly);
    out.setVersion((QDataStream::Qt_4_7));
    out<<quint16(0)<<str_mess_from.user_name<<str_mess_from.type<<str_mess_from.x<<str_mess_from.y<<str_mess_from.angle<<str_mess_from.Health_I<<str_mess_from.Health_You;

    qDebug()<<str_mess_from.Health_I+" Health "+ str_mess_from.Health_You;

    out.device()->seek(0);
    out<<quint16(arrBlock.size()-sizeof(quint16));
    pScoket->write(arrBlock);
    str_mess_from.type="";
}


void Server::slotNewConnection()
{
    connected=true;
    qDebug()<<"New Connection to server";
    QTcpSocket* pClientSocket=m_ptcpServer->nextPendingConnection();
    connect(pClientSocket,SIGNAL(disconnected()),pClientSocket,SLOT(deleteLater()));
    connect(pClientSocket,SIGNAL(readyRead()),this,SLOT(slotReadClient()));
    sendToClient(pClientSocket,"Server Response: Connected!");
    //sendToClient(pClientSocket);
}

void Server::slotReadClient()
{
    reading=true;
    qDebug()<<"Server read";
    QTcpSocket* pClientSocket=(QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion((QDataStream::Qt_4_7));
    for(;;)
    {
        if(!m_nNextBlockSize)
        {
            if(pClientSocket->bytesAvailable()<sizeof(quint16))break;
            in>>m_nNextBlockSize;
        }

//    if(pClientSocket->bytesAvailable()<m_nNextBlockSize) break;

    in>>str_mess_for.user_name>>str_mess_for.type>>str_mess_for.x>>str_mess_for.y>>str_mess_for.angle>>str_mess_for.Health_I>>str_mess_for.Health_You;
    m_nNextBlockSize=0;
    sendToClient(pClientSocket);
    }
}
