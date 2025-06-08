#include "DatabaseWriter.h"
#include <nlohmann/json.hpp>
#include <QDebug>

DatabaseWriter::DatabaseWriter(QObject* parent) : QObject(parent), conn_(nullptr) {}

DatabaseWriter::~DatabaseWriter() {
    stop();
}

void DatabaseWriter::start(const QString& connectionString) {
    conn_ = PQconnectdb(connectionString.toUtf8().data());
    if (PQstatus(conn_) != CONNECTION_OK) {
        qWarning() << "PostgreSQL connection failed:" << PQerrorMessage(conn_);
        PQfinish(conn_);
        conn_ = nullptr;
        return;
    }

    running_ = true;
    connect(&workerThread_, &QThread::started, this, &DatabaseWriter::writerLoop);
    moveToThread(&workerThread_);
    workerThread_.start();
}

void DatabaseWriter::stop() {
    running_ = false;
    dataAvailable_.wakeAll();
    workerThread_.quit();
    workerThread_.wait();

    if (conn_) {
        PQfinish(conn_);
        conn_ = nullptr;
    }
}

void DatabaseWriter::enqueue(const QString& jsonMessage) {
    QMutexLocker locker(&mutex_);
    queue_.enqueue(jsonMessage);
    dataAvailable_.wakeOne();
}

void DatabaseWriter::writerLoop() {
    while (running_) {
        QString message;

        {
            QMutexLocker locker(&mutex_);
            if (queue_.isEmpty()) {
                dataAvailable_.wait(&mutex_);
                continue;
            }
            message = queue_.dequeue();
        }

        processMessage(message);
    }
}

void DatabaseWriter::processMessage(const QString& jsonStr) {
    using json = nlohmann::json;

    try {
        auto parsed = json::parse(jsonStr.toStdString());

        QString instrumentId = QString("%1_%2")
                                   .arg(QString::fromStdString(parsed["exchange"]))
                                   .arg(QString::fromStdString(parsed["instrument"]["symbol"]));
        double bid = parsed["bid"].is_null() ? 0.0 : parsed["bid"].get<double>();
        double ask = parsed["ask"].is_null() ? 0.0 : parsed["ask"].get<double>();
        qint64 timestamp = parsed["timestamp"];

        QString query = QString(R"(
            INSERT INTO market_data (id, ts, bid, ask)
            VALUES ('%1', to_timestamp(%2 / 1000.0), %3, %4)
        )")
                            .arg(instrumentId)
                            .arg(timestamp)
                            .arg(bid)
                            .arg(ask);

        PGresult* res = PQexec(conn_, query.toUtf8().data());
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            qWarning() << "PostgreSQL insert failed:" << PQerrorMessage(conn_);
        }
        PQclear(res);
        

    } catch (const std::exception& ex) {
        qWarning() << "DatabaseWriter JSON parse error:" << ex.what();
    }
}
