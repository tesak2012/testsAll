#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include "server.h"
#include "client.h"

class ClientServerTest : public QObject
{
    Q_OBJECT
    
public:
    ClientServerTest();
    
private Q_SLOTS:

    void testCreateServer();
    void testConnectToServer();
    void testSendServer();
    void testReadServer();
    void testConnectingClient();
    void testSendClient();
    void testClientServer();
    void testCreateUser();
    void testCreateUser_data();
};

ClientServerTest::ClientServerTest()
{
}

void ClientServerTest::testCreateServer()
{
    Server *server=new Server(135);
    QCOMPARE(server->m_ptcpServer->isListening(),false);
    server->m_ptcpServer->close();
    server=new Server(3333);
    QCOMPARE(server->m_ptcpServer->isListening(),true);
    server->m_ptcpServer->close();


}

void ClientServerTest::testConnectToServer()
{
        Server *server=new Server(33333);
        QTcpSocket *_client=new QTcpSocket();
        _client->connectToHost(QHostAddress("127.0.0.1"),33333);
        while (!server->connected)
            qApp->processEvents();
        QVERIFY(server->connected==true);
        server->m_ptcpServer->close();
\
}

void ClientServerTest::testSendServer()
{
    Server *server=new Server(3333);
    QTcpSocket *_client=new QTcpSocket();
    _client->connectToHost(QHostAddress("127.0.0.1"),3333);
    QDataStream in(_client);
    in.setVersion(QDataStream::Qt_4_7);
    while(!server->connected)
        qApp->processEvents();
    QString answer,sa1;
    QString text="Server Response: Connected!";
    in>>answer;
    in>>answer;
    QVERIFY(answer.contains(text)==true);
    server->m_ptcpServer->close();

}

void ClientServerTest::testReadServer()
{
    Server *server=new Server(3333);
    QTcpSocket *_client=new QTcpSocket();
    _client->connectToHost(QHostAddress("127.0.0.1"),3333);
    while(!server->connected)
        qApp->processEvents();

    QByteArray arrBlock;
    QDataStream out(&arrBlock,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    server->str_mess_from.user_name="asdasdasf";
    out<<quint16(0)<<server->str_mess_from.user_name;

    out.device()->seek(0);
    out<<quint16(arrBlock.size()-sizeof(quint16));
    _client->write(arrBlock);

    while(!server->reading)
        qApp->processEvents();

    QVERIFY(server->reading==true);
    server->m_ptcpServer->close();
}

void ClientServerTest::testConnectingClient()
{
    QTcpServer *_server=new QTcpServer();
    _server->listen(QHostAddress::Any,3333);
    Client *client=new Client(QHostAddress("127.0.0.1"),3333,"username");
    while(!client->connected)
        qApp->processEvents();
    QVERIFY(client->m_pTcpSocket->state()== QTcpSocket::ConnectedState);
    QVERIFY(client->connected==true);
    _server->close();
    client->m_pTcpSocket->close();
}

void ClientServerTest::testSendClient()
{
    QTcpServer *_server=new QTcpServer();
    _server->listen(QHostAddress::Any,3333);
    Client *client=new Client(QHostAddress("127.0.0.1"),3333,"username");
    QString str="asfasf";
    client->slotSendToServer(str);
    while(!client->connected)
        qApp->processEvents();

    _server->close();
    client->m_pTcpSocket->close();

    //QTcpSocket* pClientSocket=(QTcpSocket*)_server->sender();

   ;



}

void ClientServerTest::testClientServer()
{
    Server *server=new Server(33333);
    Client *client=new Client(QHostAddress("127.0.0.1"),33333,"username");
    while(!client->connected&&!server->connected)
        qApp->processEvents();


    client->str_mess_from.user_name="ssa";

    client->slotSendToServer(client->str_mess_from);


    while(!client->reading)
    {
        qApp->processEvents();
    }

    while(!server->reading)
    {
        qApp->processEvents();
    }

    QCOMPARE(server->str_mess_for.user_name,client->str_mess_for.user_name);

}

//немного не в тему
void ClientServerTest::testCreateUser()
{
    Client *client=new Client(QHostAddress("127.0.0.1"),33333,"Anton");
    QFETCH(QString,UserName);
    QCOMPARE(client->user_name,UserName);

}

void ClientServerTest::testCreateUser_data()
{

    QTest::addColumn<QString>("UserName");
    QTest::newRow("0") << "Anton";

}

QTEST_MAIN(ClientServerTest)

#include "tst_clientservertest.moc"
