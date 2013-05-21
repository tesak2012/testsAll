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
    void testSendFromServer();
    void testReadServer();
    void testConnectClient();
    void testSendFromClient();
    void testClientServer();
    void testCreateUser();
    void testCreateUser_data();
};

ClientServerTest::ClientServerTest()
{
}

void ClientServerTest::testCreateServer()
{
    Server *server=new Server(33331);
    QCOMPARE(server->m_ptcpServer->isListening(),true);
    server->m_ptcpServer->close();
}

void ClientServerTest::testConnectToServer()
{
        Server *server=new Server(33332);
        QTcpSocket *_client=new QTcpSocket();
        _client->connectToHost(QHostAddress("127.0.0.1"),33332);
        while (!server->connected)
            qApp->processEvents();
        QVERIFY(server->connected==true);
        server->m_ptcpServer->close();
\
}

void ClientServerTest::testSendFromServer()
{
    Server *server=new Server(33333);
    QTcpSocket *_client=new QTcpSocket();
    _client->connectToHost(QHostAddress("127.0.0.1"),33333);
    QDataStream in(_client);
    in.setVersion(QDataStream::Qt_4_7);
    while(!server->connected)
        qApp->processEvents();
    QString answer;
    QString text="Server Response: Connected!";
    while(!answer.contains(text))
    in>>answer;
    QVERIFY(answer.contains(text)==true);
    server->m_ptcpServer->close();

}

void ClientServerTest::testReadServer()
{
    Server *server=new Server(33334);
    QTcpSocket *_client=new QTcpSocket();
    _client->connectToHost(QHostAddress("127.0.0.1"),33334);
    while(!server->connected)
        qApp->processEvents();

    QByteArray arrBlock;
    QDataStream out(&arrBlock,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);

    out<<quint16(0)<<"Message";

    out.device()->seek(0);
    out<<quint16(arrBlock.size()-sizeof(quint16));
    _client->write(arrBlock);

    while(!server->reading)
        qApp->processEvents();

    QVERIFY(server->reading==true);
    server->m_ptcpServer->close();
}

void ClientServerTest::testConnectClient()
{
    QTcpServer *_server=new QTcpServer();
    _server->listen(QHostAddress::Any,33335);
    Client *client=new Client(QHostAddress("127.0.0.1"),33335,"username");
    while(!client->connected)
        qApp->processEvents();
    QVERIFY(client->m_pTcpSocket->state()== QTcpSocket::ConnectedState);
    QVERIFY(client->connected==true);
    _server->close();
    client->m_pTcpSocket->close();
}

void ClientServerTest::testSendFromClient()
{
    QTcpServer *_server=new QTcpServer();
    _server->listen(QHostAddress::Any,33336);
    Client *client=new Client(QHostAddress("127.0.0.1"),33336,"username");
    QString str="Message";
    client->slotSendToServer(str);
    while(!client->connected)
        qApp->processEvents();
    while(!client->reading)
        qApp->processEvents();
    _server->close();
    client->m_pTcpSocket->close();

    //QTcpSocket* pClientSocket=(QTcpSocket*)_server->sender();

   ;



}

void ClientServerTest::testClientServer()
{
    Server *server=new Server(33337);
    Client *client=new Client(QHostAddress("127.0.0.1"),33337,"username");
    while(!client->connected&&!server->connected)
        qApp->processEvents();


    client->str_mess_from.user_name="Anton";

    client->slotSendToServer(client->str_mess_from);


    while(!client->reading)
    {
        qApp->processEvents();
    }

    while(!server->reading)
    {
        qApp->processEvents();
    }

    QCOMPARE(server->str_mess_for.user_name,client->str_mess_from.user_name);

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
