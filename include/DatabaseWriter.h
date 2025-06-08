#pragma once

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QString>
#include <libpq-fe.h>

class DatabaseWriter : public QObject {
    Q_OBJECT

public:
    explicit DatabaseWriter(QObject* parent = nullptr);
    ~DatabaseWriter();

    void start(const QString& connectionString);
    void stop();
    void enqueue(const QString& jsonMessage);

private:
    void writerLoop();
    void processMessage(const QString& json);

    PGconn* conn_;
    QThread workerThread_;
    QQueue<QString> queue_;
    QMutex mutex_;
    QWaitCondition dataAvailable_;
    bool running_ = false;
};
