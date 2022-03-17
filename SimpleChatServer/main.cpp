#include <QCoreApplication>
#include <chatserver.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ChatServer *server = new ChatServer();

    if(!server->listen(QHostAddress::LocalHost, 1967)){
        std::cout << "Server did not start\n";
    } else {
        std::cout <<  "Server is listening at ip: " << server->serverAddress().toString().toStdString() << " at port: " << server->serverPort() << "\n";
    }

    return a.exec();
}

