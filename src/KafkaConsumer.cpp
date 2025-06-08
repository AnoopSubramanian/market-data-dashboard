#include "KafkaConsumer.h"
#include <nlohmann/json.hpp>
#include <QDebug>
#include <iostream>

KafkaConsumer::KafkaConsumer(QObject* parent)
    : QObject(parent), running_(false) {}

KafkaConsumer::~KafkaConsumer() {
    stop();
}

void KafkaConsumer::start(const std::string& brokers, const std::string& topic) {
    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    std::string errStr;

    conf->set("bootstrap.servers", brokers, errStr);
    conf->set("group.id", "market-data-dashboard", errStr);
    conf->set("auto.offset.reset", "latest", errStr);

    consumer_.reset(RdKafka::KafkaConsumer::create(conf, errStr));
    delete conf;

    consumer_->subscribe({topic});
    running_ = true;

    consumerThread_ = std::thread([this]() {
        while (running_) {
            auto msg = consumer_->consume(1000);
            if (msg->err() == RdKafka::ERR_NO_ERROR) {
                QString json = QString::fromStdString(std::string(
                        static_cast<const char*>(msg->payload()), msg->len()));
                std::cout << "Message received: " << msg->payload() << std::endl;
                emit messageReceived(json);
                qDebug() << "[KafkaConsumer] Emitting messageReceived signal.";
            }
            delete msg;
        }
    });
}

void KafkaConsumer::stop() {
    running_ = false;
    if (consumerThread_.joinable()) consumerThread_.join();
    if (consumer_) consumer_->close();
}
