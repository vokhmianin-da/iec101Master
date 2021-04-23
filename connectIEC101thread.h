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
    CS101_Master master = nullptr;   //соединение IEC101Master
    SerialPort port = nullptr;
    QString serialPort = "COM13";
    QQueue <InformationObject> commandQueue;    //очередь команд для отправки по IEC101
    bool running = false; //флаг наличия соединения
    sLinkLayerParameters llParameters;  //параметры соединения
    uint16_t commTimeout = 50;
    bool useSingleCharAck = true;

public:
    ConnectIEC101Thread(QString port);
    bool isConnect();
    static void linkLayerStateChanged(void* parameter, int address, LinkLayerState state);
    static bool asduReceivedHandler (void* parameter, int address, CS101_ASDU asdu);
    static void sigint_handler(int signalId, void* ptr)
    {
        ConnectIEC101Thread *ptrConnectIEC101Thread = static_cast<ConnectIEC101Thread*>(ptr);
        ptrConnectIEC101Thread->running = false;
    }

signals:
    void setTextStatus(QString);    //сигнал для записи статуса соединения
    void getIEC101Info(int addr, int value);    //сигнал о получении данных
    void closeConnection(); //сигнал о закрытии текущего соединения

private slots:
    void commandIOformation(int addr, QVariant val, IEC60870_5_TypeID commandType); //создание InformationsObject для последующей отправки в методе run()
    void disconnect();  //закрытие потока и освобождение памяти

protected:
    void run() override;
};

#endif // CONNECTIEC101THREAD_H
