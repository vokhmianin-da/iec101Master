#ifndef CONNECTIEC101THREAD_H
#define CONNECTIEC101THREAD_H

#include <QThread>
#include <QObject>
#include <QVariant>
#include <QQueue>

#include "iec60870/iec60870_master.h"
#include "iec60870/iec60870_common.h"
#include "iec60870/hal_time.h"
#include "iec60870/hal_thread.h"
#include "iec60870/hal_serial.h"
#include "iec60870/cs101_master.h"


class ConnectIEC101Thread : public QThread
{
    Q_OBJECT

private:
    QString ipIEC104;
    uint16_t portIEC104;
    CS101_Master master = nullptr;   //соединение IEC101Master
    SerialPort port = nullptr;
    QQueue <InformationObject> commandQueue;    //очередь команд для отправки по IEC101
    bool isRun = false; //флаг наличия соединения

public:
    ConnectIEC101Thread(QString ip, uint16_t port);
    bool isConnect();
    static void linkLayerStateChanged(void* parameter, int address, LinkLayerState state);
    static bool asduReceivedHandler (void* parameter, int address, CS101_ASDU asdu);

signals:
    void setTextStatus(QString);    //сигнал для записи статуса соединения
    void getIEC104Info(int addr, int value);    //сигнал о получении данных
    void closeConnection(); //сигнал о закрытии текущего соединения

private slots:
    void commandIOformation(int addr, QVariant val, IEC60870_5_TypeID commandType); //создание InformationsObject для последующей отправки в методе run()
    void disconnect();  //закрытие потока и освобождение памяти

protected:
    void run() override;
};

#endif // CONNECTIEC101THREAD_H
