#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bluetoothscanner.h"
#include "manualcontrol.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_M_connectButton_clicked();
    void on_M_disconnectButton_clicked();
    void on_ManualControlcheckBox_toggled(bool checked);
    void updateGyro(QString X, QString Y);
    void updateReflex(double X, double Y);
    void updateAvst(double X, double Y);


    void on_M_sendButton_clicked();

    void on_debugg_clicked();

private:
    Ui::MainWindow *ui;
    BluetoothScanner *bt;
    ManualControl *mc;
};

#endif // MAINWINDOW_H
