#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "QTcpServer"
#include <QTcpSocket>

struct structure_message_s
{
   QString x;
   QString y;
   QString type;
   QString user_name;
   QString angle;
   QString Health_I;
   QString Health_You;
};

class Server : public QObject
{
    Q_OBJECT
public:
    //explicit Server(QObject *parent = 0);
    Server(int nPort,QObject *parent = 0);
    ~Server();
public:

    structure_message_s str_mess_from;
    structure_message_s str_mess_for;
    QTcpServer* m_ptcpServer;
    quint16 m_nNextBlockSize;
    bool connected;
    bool reading;
    void sendToClient(QTcpSocket* pScoket,const QString& str);

public slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void sendToClient(QTcpSocket* pScoket);
    
};

#endif // SERVER_H
