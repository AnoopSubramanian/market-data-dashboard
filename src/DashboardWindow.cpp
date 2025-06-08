#include "DashboardWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QDebug>

DashboardWindow::DashboardWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    loadDropdownOptions();

    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &DashboardWindow::refreshDashboard);
    refreshTimer->start(1000); // refresh every second
}

void DashboardWindow::setupUI() {
    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);

    exchangeCombo = new QComboBox();
    instrumentTypeCombo = new QComboBox();
    instrumentCombo = new QComboBox();
    lookbackWindowCombo = new QComboBox();

    layout->addWidget(new QLabel("Exchange"));
    layout->addWidget(exchangeCombo);
    layout->addWidget(new QLabel("Instrument Type"));
    layout->addWidget(instrumentTypeCombo);
    layout->addWidget(new QLabel("Instrument"));
    layout->addWidget(instrumentCombo);
    layout->addWidget(new QLabel("Lookback Window"));
    layout->addWidget(lookbackWindowCombo);

    marketDataTable = new QTableWidget(0, 5);
    marketDataTable->setHorizontalHeaderLabels({"Timestamp", "Price", "Bid", "Ask", "Volume"});
    marketDataTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(marketDataTable);

    setCentralWidget(central);
}

void DashboardWindow::loadDropdownOptions() {
    // TODO: Load from PostgreSQL
    exchangeCombo->addItems({"Bybit", "Kraken"});
    instrumentTypeCombo->addItems({"Spot", "Futures", "Options", "Perpetual"});
    lookbackWindowCombo->addItems({"1 min", "5 min", "15 min", "1 hour", "1 day"});
}

void DashboardWindow::refreshDashboard() {
    loadMarketData();  // will populate marketDataTable
}

void DashboardWindow::loadMarketData() {
    // TODO: Query PostgreSQL based on filters and fill table
    qDebug() << "Fetching data for:"
             << exchangeCombo->currentText()
             << instrumentTypeCombo->currentText()
             << instrumentCombo->currentText()
             << lookbackWindowCombo->currentText();
}
