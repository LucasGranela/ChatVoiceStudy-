#include "chatserver.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>

ChatServer::ChatServer(QObject *parent) : QTcpServer(parent) {

}

void ChatServer::incomingConnection(qintptr socketDescriptor) {

    ServerWorker *worker = new ServerWorker(this);
    if (!worker->setSocketDescriptor(socketDescriptor)) {
        worker->deleteLater();
        return;
    }
    connect(worker, &ServerWorker::disconnectedFromClient, this, std::bind(&ChatServer::slotUserDisconnected, this, worker));
    connect(worker, &ServerWorker::error, this, std::bind(&ChatServer::slotUserError, this, worker));
    connect(worker, &ServerWorker::jsonReceived, this, std::bind(&ChatServer::jsonReceived, this, worker, std::placeholders::_1));
    connect(worker, &ServerWorker::logMessage, this, &ChatServer::slotLogMessage);
    m_clients.append(worker);
    slotLogMessage(QStringLiteral("New client Connected"));
}

void ChatServer::sendSocketToCanal(ServerWorker *user, QString canal) {
    Q_ASSERT(user);
    ServerCanal* newCanal = nullptr;

    for (ServerCanal *currentCanal : qAsConst(m_canal)) {
        if (canal.compare(currentCanal->getNumberCanal(), Qt::CaseInsensitive) == 0)
            newCanal = currentCanal;
    }

    if(newCanal == nullptr){
        newCanal = new ServerCanal();
        newCanal->setNumberCanal(canal);
        m_canal.append(newCanal);
    }

    disconnect(user, SIGNAL(disconnectedFromClient(ServerWorker)), this, SLOT(slotUserDisconnected(ServerWorker)));
    disconnect(user, SIGNAL(error(ServerWorker)), this, SLOT(slotUserError(ServerWorker)));
    disconnect(user, SIGNAL(jsonReceived(ServerWorker, QJsonObject)), this, SLOT(jsonReceived(ServerWorker, QJsonObject)));
    disconnect(user, SIGNAL(logMessage(QString)), this, SLOT(slotLogMessage(QString)));

    newCanal->setMemberCanal(user);

    m_clients.removeAll(user); //it delete on the list but not the pointer
}



void ChatServer::jsonReceived(ServerWorker *sender, const QJsonObject &doc)
{
    Q_ASSERT(sender);
    emit logMessage(QLatin1String("JSON received ") + QString::fromUtf8(QJsonDocument(doc).toJson()));
    if (sender->userName().isEmpty())
        return jsonFromLoggedOut(sender, doc);
    jsonFromLoggedIn(sender, doc);
}

void ChatServer::stopServer()
{
    for (ServerWorker *worker : m_clients) {
        worker->disconnectFromClient();
    }
    close();
}

void ChatServer::slotUserDisconnected(ServerWorker *sender) {
    userDisconnected(sender);
}

void ChatServer::slotUserError(ServerWorker *sender) {
    userError(sender);
}

void ChatServer::slotLogMessage(const QString &msg) {
    logMessage(msg);
}

void ChatServer::jsonFromLoggedOut(ServerWorker *sender, const QJsonObject &docObj)
{
    Q_ASSERT(sender);
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return;
    if (typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) != 0)
        return;
    const QJsonValue canalVal = docObj.value(QLatin1String("canal"));
    if(canalVal.isNull() || typeVal.toString().compare(QLatin1String("0"), Qt::CaseInsensitive) == 0) {
        QJsonObject message;
        message[QStringLiteral("type")] = QStringLiteral("login");
        message[QStringLiteral("success")] = false;
        message[QStringLiteral("reason")] = QStringLiteral("canal Invalid");
        sendJson(sender, message);
        return;
    }
    const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
    if (usernameVal.isNull() || !usernameVal.isString())
        return;
    const QString newUserName = usernameVal.toString().simplified();
    if (newUserName.isEmpty())
        return;
    for (ServerWorker *worker : qAsConst(m_clients)) {
        if (worker == sender)
            continue;
        if (worker->userName().compare(newUserName, Qt::CaseInsensitive) == 0) {
            QJsonObject message;
            message[QStringLiteral("type")] = QStringLiteral("login");
            message[QStringLiteral("success")] = false;
            message[QStringLiteral("reason")] = QStringLiteral("duplicate username");
            sendJson(sender, message);
            return;
        }
    }
    sender->setUserName(newUserName);
    QJsonObject successMessage;
    successMessage[QStringLiteral("type")] = QStringLiteral("login");
    successMessage[QStringLiteral("success")] = true;
    successMessage[QStringLiteral("canal")] = canalVal;
    sendJson(sender, successMessage);
    QJsonObject connectedMessage;
    connectedMessage[QStringLiteral("type")] = QStringLiteral("newuser");
    connectedMessage[QStringLiteral("username")] = newUserName;
    broadcast(connectedMessage, sender);

    sendSocketToCanal(sender, canalVal.toString());
}
