#include "manualcontrol.h"
#include "ui_manualcontrol.h"

ManualControl::ManualControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManualControl)
{
    //bs = new BluetoothScanner();
    ui->setupUi(this);


}


ManualControl::ManualControl(BluetoothScanner *bs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManualControl)
{
    bt = bs;
    qDebug()<< "constructor"<<&bs<<endl;
    qDebug()<< "constructor"<<&bt<<endl;

    ui->setupUi(this);
}

ManualControl::~ManualControl()
{
    delete ui;
}
void ManualControl::keyPressEvent( QKeyEvent * event )
{

    if(event->key()==Qt::Key_W)
    {
        qDebug()<< "keyPressed"<<&bt<<endl;
        bt->sendMessage("w");
    }

}
void ManualControl::keyReleaseEvent(QKeyEvent *event)
{
    bt->sendMessage("0");
}

