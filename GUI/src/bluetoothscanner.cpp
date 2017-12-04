#include "bluetoothscanner.h"
#include "ui_bluetoothscanner.h"
#include <QDebug>
#include <QIODevice>


BluetoothScanner::BluetoothScanner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BluetoothScanner)
{

    ui->setupUi(this);
    socket= new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol,this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(disconnected()),this,SLOT(disconnected()));


}

BluetoothScanner::~BluetoothScanner()
{
    delete ui;
}

void BluetoothScanner::on_BTListWidget_itemClicked(QListWidgetItem *item)
{
    slaveBT= item->text();
}

/*
 * handles the event when the SCAN button i clicked
 * @return void
 * clears the list of BT device and shows the newly discovered ones
 * start scaning for BT devices, once found calls foudDevices()
 *
*/
void BluetoothScanner::on_BTScanButton_clicked()
{
    BTdiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    ui->BTListWidget->clear();

    //Starts looking for bluetooth devices and notfices which in turn prints
    //them out to the consel
    connect(BTdiscoveryAgent,SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this,SLOT(foundDevices(QBluetoothDeviceInfo)));
    //search for nearby devices for 15000 sm
    BTdiscoveryAgent->setLowEnergyDiscoveryTimeout(15000);
    BTdiscoveryAgent->start();

}

void BluetoothScanner::on_BTConnectButton_clicked()
{
    //socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    for (QBluetoothDeviceInfo i: nearbyDevices)
    {
        if(i.name().contains(slaveBT))
        {
            socket->connectToService(i.address(),i.deviceUuid(), QIODevice::ReadWrite);
            BTdiscoveryAgent->stop();
            emit connectedToDevice();
            on_BTOkButton_clicked();
            break;
        }
        continue;
    }
}

void BluetoothScanner::on_BTOkButton_clicked()
{
    this->close();
}
void BluetoothScanner::foundDevices(QBluetoothDeviceInfo info)
{
    if(info.name().length()>1)
    {
        qDebug()<<"Bluetooth device:  "<< info.name()<< "address ( "<< info.address().toString()<< ")";
        qDebug()<<"Bluetooth device:  "<<info.deviceUuid();
        //qDebug()<< "BT"<<info.add;
        if(info.name()!=slaveBT)
        {
            ui->BTListWidget->addItem(info.name());
            nearbyDevices.append(info);
            slaveBT =info.name();
        }

    }

}


void BluetoothScanner::readSocket()
{
    qDebug()<<endl<<endl<<"READ SOCKET"<<endl<<endl;
    qDebug()<<endl<<endl<<socket->isOpen()<<endl<<endl;

    if (!socket)
        return;

    while (socket->canReadLine())
    {
        QByteArray line = socket->readLine().trimmed();
        qDebug()<< QString::fromUtf8(line.constData(), line.length())<<endl;
        sendMessage(QString::fromUtf8(line.constData(), line.length()));
    }
}
void BluetoothScanner::sendMessage(QString text)
{
 /*   QByteArray arr= text.toUtf8()+ '\n';
    socket->write(arr);
    while (socket->canReadLine()) {
        QByteArray line = this->socket->readAll();
        qDebug()<< (QString::fromUtf8(line.constData()), line.length())<<endl;
*/
    QByteArray w= text.toUtf8();
    socket->write(w);
}
void BluetoothScanner::disconnected()
{
    qDebug()<<slaveBT+" has disconnected"<<endl;
}
void BluetoothScanner::disconnectfromService()
{
   socket->disconnectFromService();
   connect(socket,SIGNAL(disconnected()),this, SLOT(disconnected()));
}

void BluetoothScanner::sendW()
{
    qDebug()<<"called"<<endl;
    qDebug()<<"can read from socket: "<< socket->canReadLine();
    qDebug()<< "can write to socket"<<socket->canReadLine();
    QString w= "w";
    QByteArray chars= w.toUtf8();
    socket->write(chars);
    QByteArray re= socket->readAll();
    QString s = QString::fromUtf8(re,re.length());
    qDebug()<<s<<endl;
}
