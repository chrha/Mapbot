#ifndef MANUALCONTROL_H
#define MANUALCONTROL_H

#include <QDialog>
#include "bluetoothscanner.h"
#include <QKeyEvent>
#include "bluetoothscanner.h"
namespace Ui {
class ManualControl;
}

class ManualControl : public QDialog
{
    Q_OBJECT


public:
    explicit ManualControl(QWidget *parent = 0);
     ManualControl(BluetoothScanner *bs,QWidget *parent= 0);
     void keyPressEvent( QKeyEvent * event );
     void keyReleaseEvent(QKeyEvent *event);
    ~ManualControl();
signals:
      void messageRecieved(QString msg);

private:
    Ui::ManualControl *ui;
    BluetoothScanner *bt;
};

#endif // MANUALCONTROL_H
