#pragma once

#include <QObject>
#include <QString>
#include <librdkafka/rdkafkacpp.h>
#include <memory>
#include <thread>
#include <atomic>

class KafkaConsumer : public QObject {
    Q_OBJECT

public:
    explicit KafkaConsumer(QObject* parent = nullptr);
    ~KafkaConsumer();

    void start(const std::string& brokers, const std::string& topic);
    void stop();

signals:
    void messageReceived(const QString& json);

private:
    std::unique_ptr<RdKafka::KafkaConsumer> consumer_;
    std::thread consumerThread_;
    std::atomic_bool running_;
};
