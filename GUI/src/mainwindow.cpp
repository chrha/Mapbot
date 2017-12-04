#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "manualcontrol.h"
#include <QThread>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bt = new BluetoothScanner();

    mc = new ManualControl(bt);
    ui->M_disconnectButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_M_connectButton_clicked()
{
    ui->M_connectButton->setEnabled(false);
    ui->M_disconnectButton->setEnabled(true);
    bt->exec();
}

void MainWindow::on_M_disconnectButton_clicked()
{
    bt->disconnectfromService();
    ui->M_connectButton->setDisabled(false);
    ui->M_disconnectButton->setDisabled(true);
}

void MainWindow::on_ManualControlcheckBox_toggled(bool checked)
{
    if (checked)
    {
        qDebug()<<"i was checked";
        mc->exec();
        if(mc->close() )
            ui->ManualControlcheckBox->setChecked(false);
    }
}



void MainWindow::on_M_sendButton_clicked()
{

    bt->sendMessage("g");
    //connect(bt,SIGNAL(connectedToDevice()),this,SLOT(updateGyro(QString,QString)));
    /*bt->sendMessage(ui->lineEdit->text().toUtf8());
   QString x= ui->lineEdit->text().toUtf8();
   updateGyro(x,"HEY");
   ui->lineEdit->clear();*/
}
void MainWindow::updateGyro(QString X, QString Y)
{
    ui->gyroscope->setText("connected");
}
void MainWindow::updateReflex(double X, double Y){}
void MainWindow::updateAvst(double X, double Y){}

void MainWindow::on_debugg_clicked()
{

    bt->sendW();

}
