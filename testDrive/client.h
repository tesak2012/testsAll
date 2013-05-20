#ifndef CLIENT_H
#define CLIENT_H

#include "QTime"
#include <QTcpSocket>
#include "QHostAddress"

#include "Struct_message.h"

//struct structure_message_cl
//{
//   QString x;
//   QString y;
//   QString type;
//   QString user_name;
//   QString angle;
//   QString Health;

//};

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
      Client(QHostAddress host_addr,int nPort,QString _user_name,QObject* pwgt=0);
      ~Client();
      QString user_name;
      structure_message str_mess_from;
      structure_message str_mess_for;
      bool connected,reading;
      QTcpSocket* m_pTcpSocket;

private:

    quint16 m_nNextBlockSize;

public:


signals:
    
public slots:
    void slotReadyRead(                 );
    void slotError (QAbstractSocket::SocketError);
    void slotSendToServer(QString &str);
    void slotConnected (                );
     void slotCo (                );
    void slotSendToServer(structure_message str_mess);
};

#endif // CLIENT_H
