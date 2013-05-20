#include "client.h"
#include "QTcpSocket"
#include "QHostAddress"
#include <QObject>

Client::Client(QObject *parent) :
    QObject(parent)
{
}

Client::Client(QHostAddress host_addr, int nPort,QString _user_name, QObject *pwgt):QObject(pwgt)
{
    connected=false;
    reading=true;
    qDebug()<<"Client socket create";
    user_name=_user_name;
    m_pTcpSocket=new QTcpSocket(this);
    connect(m_pTcpSocket,SIGNAL(connected()),SLOT(slotConnected()));
    connect(m_pTcpSocket,SIGNAL(readyRead()),SLOT(slotReadyRead()));
    connect(m_pTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotError(QAbstractSocket::SocketError)));
    m_pTcpSocket->connectToHost(host_addr,nPort);
}

Client::~Client()
{
}

void Client::slotReadyRead()
{
    reading=true;
     qDebug()<<"Client read";
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_4_7);
    for (;;) {
        if(!m_nNextBlockSize){
            if(m_pTcpSocket->bytesAvailable()<sizeof(quint16))
            {
                break;
            }
            in>>m_nNextBlockSize;
        }
        if(m_pTcpSocket->bytesAvailable()<m_nNextBlockSize)
        {
            break;
        }

QString str;
qDebug()<<"Client reading";
in>>str_mess_for.user_name>>str_mess_for.type>>str_mess_for.x>>str_mess_for.y>>str_mess_for.angle>>str_mess_for.Health_I>>str_mess_for.Health_You;
qDebug()<<"Server has said "+str_mess_for.user_name+", type "+str_mess_for.type+", x "+str_mess_for.x+", y "+str_mess_for.y+"-*-"+str_mess_for.angle +" Health_I "+str_mess_for.Health_I+" Helth_You "+str_mess_for.Health_You;
//m_ptxtInfo->append(time.toString()+" "+str);
m_nNextBlockSize=0;
    }
}

void Client::slotError(QAbstractSocket::SocketError err)
{
    QString strError=
            "Error: "+(err==QAbstractSocket::HostNotFoundError?
                "The host was not found":
                err==QAbstractSocket::RemoteHostClosedError?
                    "The remote host is closed":
                    err==QAbstractSocket::ConnectionRefusedError?
                        "The connection was refused":
                        QString(m_pTcpSocket->errorString())
                        );
 //   m_ptxtInfo->append(strError);
}

void Client::slotSendToServer(QString &str)

{
    QByteArray arrBlock;
    QDataStream out(&arrBlock,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);

    out.setVersion(QDataStream::Qt_4_7);
    out<<quint16(0)<<str;
    out.device()->seek(0);
    out<<quint16(arrBlock.size()-sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
   // m_ptxtInput->setText("");
}

void Client::slotSendToServer(structure_message str_mess)
{
    qDebug()<<"Client send to server";
    QByteArray arrBlock;
    QDataStream out(&arrBlock,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out<<quint16(0)<<str_mess.user_name<<str_mess.type<<str_mess.x<<str_mess.y<<str_mess.angle<<str_mess.Health_I<<str_mess.Health_You;

    out.device()->seek(0);
    out<<quint16(arrBlock.size()-sizeof(quint16));

    m_pTcpSocket->write(arrBlock);

   // m_ptxtInput->setText("");
}
void Client::slotConnected()
{

    qDebug()<<"Client connected";
    connected=true;

}

void Client::slotCo()
{

}
