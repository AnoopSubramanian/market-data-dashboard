#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QTableWidget>
#include <QDateTime>
#include <QTimer>

class DashboardWindow : public QMainWindow {
    Q_OBJECT

public:
    DashboardWindow(QWidget* parent = nullptr);

private slots:
    void refreshDashboard();

private:
    QComboBox* exchangeCombo;
    QComboBox* instrumentTypeCombo;
    QComboBox* instrumentCombo;
    QComboBox* lookbackWindowCombo;
    QTableWidget* marketDataTable;

    void setupUI();
    void loadDropdownOptions();
    void loadMarketData();

    QTimer* refreshTimer;
};

#endif // DASHBOARDWINDOW_H
