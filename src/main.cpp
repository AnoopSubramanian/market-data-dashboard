#include <QApplication>
#include "DashboardWindow.h"
#include "KafkaConsumer.h"
#include "DatabaseWriter.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    DashboardWindow window;
    window.resize(1024, 1024);
    window.show();

    KafkaConsumer* consumer = new KafkaConsumer();
    DatabaseWriter* writer = new DatabaseWriter();

    writer->start("host=localhost dbname=market user=postgres password=postgres");

    QObject::connect(consumer, &KafkaConsumer::messageReceived, writer, &DatabaseWriter::enqueue);
    consumer->start("localhost:9092", "market.ticker");

    return app.exec();
}
