#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setConnectionIEC101Master(QString port)
{
        /*Управление кнопками*/
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(true);

        if(!connectionThread) connectionThread = new ConnectIEC101Thread(port);
        connect(connectionThread, SIGNAL(setTextStatus(QString)), this, SLOT(on_setTextStatus(QString)));
        connect(this, SIGNAL(sendCom(int,QVariant,IEC60870_5_TypeID)), connectionThread, SLOT(commandIOformation(int,QVariant,IEC60870_5_TypeID)));
        connect(connectionThread, SIGNAL(getIEC101Info(int,int)), this, SLOT(receiveDataIEC101(int,int)));
        connect(this, SIGNAL(commandCloseConnection()), connectionThread, SLOT(disconnect()));
        connect(connectionThread, SIGNAL(closeConnection()), this, SLOT(closeConnectionIEC101()));

        connectionThread->start();


    //Thread_sleep(1000);

}




void MainWindow::on_pbConnect_clicked() //кнопка "Connect"
{
    setConnectionIEC101Master(ui->lePort->text());
}

void MainWindow::on_pbDisconnect_clicked()  //кнопка "Disconnect"
{
    /*Управление кнопками*/
    ui->pbConnect->setEnabled(true);
    ui->pbDisconnect->setEnabled(false);

    if(connectionThread) emit commandCloseConnection();
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)    //отправка команды по изменению значения в таблице
{
    if(column == 1) //если изменилось поле value
    {
        if(row == 0)    //Для BitString
        {
            if(connectionThread) emit sendCom(1, ui->tableWidget->item(0, 1)->text().toInt(), C_BO_NA_1);
        }
        if(row == 1)    //Для Word
        {
            if(connectionThread) emit sendCom(3, ui->tableWidget->item(1, 1)->text().toInt(), C_SE_NB_1);
        }
    }
}

void MainWindow::on_setTextStatus(QString str)
{
    ui->textEdit->append(str);
}

void MainWindow::receiveDataIEC101(int addr, int value) //прием данных по IEC104
{
    if( addr == ui->tableWidget->item(0, 0)->text().toInt())
    {
        ui->tableWidget->setItem(0, 1, new QTableWidgetItem(QString::number(value)));
    }
    else if( addr == ui->tableWidget->item(1, 0)->text().toInt())
    {
        ui->tableWidget->setItem(1, 1, new QTableWidgetItem(QString::number(value)));
    }
}

void MainWindow::closeConnectionIEC101()    // Закрытие соединения
{
    ui->textEdit->append("exit");
    disconnect(connectionThread, SIGNAL(setTextStatus(QString)), this, SLOT(on_setTextStatus(QString)));
    disconnect(this, SIGNAL(sendCom(int,QVariant,IEC60870_5_TypeID)), connectionThread, SLOT(commandIOformation(int,QVariant,IEC60870_5_TypeID)));
    disconnect(connectionThread, SIGNAL(getIEC101Info(int,int)), this, SLOT(receiveDataIEC101(int,int)));
    disconnect(this, SIGNAL(commandCloseConnection()), connectionThread, SLOT(disconnect()));
    disconnect(connectionThread, SIGNAL(closeConnection()), this, SLOT(closeConnectionIEC101()));
    connectionThread = nullptr;
}
