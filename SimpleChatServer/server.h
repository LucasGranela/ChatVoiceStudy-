#ifndef SERVER_H
#define SERVER_H

#include "serverworker.h"

class Server {

public:
    explicit Server();

protected:
    QVector<ServerWorker *> m_clients;

    void broadcast(const QJsonObject &message, ServerWorker *exclude);
    void userDisconnected(ServerWorker *sender);
    void userError(ServerWorker *sender);
    void logMessage(const QString &msg);
    void jsonFromLoggedIn(ServerWorker *sender, const QJsonObject &doc);
    void sendJson(ServerWorker *destination, const QJsonObject &message);
};

#endif // SERVER_H
