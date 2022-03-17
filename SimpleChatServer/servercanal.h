#ifndef SERVERCANAL_H
#define SERVERCANAL_H

#include <QThread>
#include <QObject>
#include "serverworker.h"
#include "server.h"

class ServerCanal : public QThread, Server
{
    Q_OBJECT
private:
    QVector<ServerWorker *> c_clients;
    QString numberCanal;

public:
    void run() override;
    explicit ServerCanal(QObject *parent = nullptr);
    QString getNumberCanal();
    void setNumberCanal(QString numberCanal);
    void setMemberCanal(ServerWorker *user);

signals:
    void canalError(const QString &msg);

private slots:
    void jsonReceived(ServerWorker *sender, const QJsonObject &doc);
    void slotUserDisconnected(ServerWorker *sender);
    void slotUserError(ServerWorker *sender);
    void slotLogMessage(const QString &msg);
};

#endif // SERVERCANAL_H
