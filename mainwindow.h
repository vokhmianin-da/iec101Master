#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVariant>

#include "iec60870/cs104_connection.h"
#include "iec60870/hal_time.h"
#include "iec60870/hal_thread.h"

#include <stdio.h>
#include <stdlib.h>

#include "connectIEC101thread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setConnectionIEC101Master(QString port);  //создание соединения от мастера

private slots:
    void on_pbConnect_clicked();

    void on_pbDisconnect_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_setTextStatus(QString str);

    /*Прием данных по IEC104*/
    void receiveDataIEC101(int addr, int value);

    /*Закрытие соединения IEC104*/
    void closeConnectionIEC101();

private:
    Ui::MainWindow *ui;
    ConnectIEC101Thread *connectionThread = nullptr;    //поток для работы с IEC101

signals:
    void sendCom(int addr, QVariant val, IEC60870_5_TypeID commandType);
    void commandCloseConnection();  //команда о закрытии соединения
};
#endif // MAINWINDOW_H
