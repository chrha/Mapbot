#ifndef BLUETOOTHSCANNER_H
#define BLUETOOTHSCANNER_H

#include <QListWidgetItem>
#include <QDialog>
#include <QBluetoothServer>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothSocket>


namespace Ui {
class BluetoothScanner;
}

class BluetoothScanner : public QDialog
{
    Q_OBJECT

public:
    explicit BluetoothScanner(QWidget *parent = 0);
    ~BluetoothScanner();
    void disconnectfromService();
    void sendMessage(QString text);
    void sendW();
signals:
    void connectedToDevice();
public slots:
    void foundDevices(QBluetoothDeviceInfo info);
    void disconnected();
    void readSocket();

private slots:
    void on_BTListWidget_itemClicked(QListWidgetItem *item);

    void on_BTScanButton_clicked();

    void on_BTConnectButton_clicked();

    void on_BTOkButton_clicked();
private:
    Ui::BluetoothScanner *ui;
    QBluetoothDeviceDiscoveryAgent *BTdiscoveryAgent;
    QList <QBluetoothDeviceInfo> nearbyDevices;
    QString slaveBT;
    QBluetoothSocket *socket;
};

#endif // BLUETOOTHSCANNER_H
