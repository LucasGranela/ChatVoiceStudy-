#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include "serverworker.h"
#include "servercanal.h"
#include "server.h"

class ChatServer : public QTcpServer, Server
{
    Q_OBJECT
    Q_DISABLE_COPY(ChatServer)

public:
    explicit ChatServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

public slots:
    void stopServer();

private slots:
    void slotUserDisconnected(ServerWorker *sender);
    void slotUserError(ServerWorker *sender);
    void slotLogMessage(const QString &msg);
    void jsonReceived(ServerWorker *sender, const QJsonObject &doc);

private:
    void jsonFromLoggedOut(ServerWorker *sender, const QJsonObject &doc);
    void sendSocketToCanal(ServerWorker *user, QString Canal);
    QVector<ServerCanal *> m_canal;
};

#endif // CHATSERVER_H
