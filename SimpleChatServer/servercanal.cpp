#include "servercanal.h"
#include <iostream>
#include <QJsonDocument>



ServerCanal::ServerCanal(QObject *parent) : QThread(parent) {

}

QString ServerCanal::getNumberCanal() {
    return this->numberCanal;
}

void ServerCanal::setNumberCanal(QString numberCanal) {
    if (this->numberCanal.isEmpty()){
        this->numberCanal = numberCanal;
    } else {
        emit canalError("The canal it was already inicialize");
    }
}

void ServerCanal::run(){

    exec();
}

void ServerCanal::setMemberCanal(ServerWorker *user) {
    connect(user, &ServerWorker::disconnectedFromClient, this, std::bind(&ServerCanal::slotUserDisconnected, this, user));
    connect(user, &ServerWorker::error, this, std::bind(&ServerCanal::slotUserError, this, user));
    connect(user, &ServerWorker::jsonReceived, this, std::bind(&ServerCanal::jsonReceived, this, user, std::placeholders::_1));
    connect(user, &ServerWorker::logMessage, this, &ServerCanal::slotLogMessage);
    m_clients.append(user);
    slotLogMessage(QStringLiteral("New client Connected"));
}

void ServerCanal::jsonReceived(ServerWorker *sender, const QJsonObject &doc) {
    Q_ASSERT(sender);
    emit logMessage(QLatin1String("JSON received ") + QString::fromUtf8(QJsonDocument(doc).toJson()));
    if (sender->userName().isEmpty())
        return;
    jsonFromLoggedIn(sender, doc);
}

void ServerCanal::slotUserDisconnected(ServerWorker *sender) {
    userDisconnected(sender);
}

void ServerCanal::slotUserError(ServerWorker *sender) {
    userError(sender);
}

void ServerCanal::slotLogMessage(const QString &msg) {
    logMessage(msg);
}
