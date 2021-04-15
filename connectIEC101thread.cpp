#include "connectIEC101thread.h"

ConnectIEC101Thread::ConnectIEC101Thread(QString ip, uint16_t port): ipIEC104(ip), portIEC104(port)
{

}

bool ConnectIEC101Thread::isConnect()
{
    if(master)
    {
        if(isRun)
        {
            return true;
        }
    }
    else
        return false;
}

void ConnectIEC101Thread::commandIOformation(int addr, QVariant val, IEC60870_5_TypeID commandType)
{
    InformationObject sc;
    switch(commandType)
    {
    case C_SC_NA_1: sc = (InformationObject)SingleCommand_create(NULL, addr, val.toBool(), false, 0);
        break;
    case C_DC_NA_1: sc = (InformationObject)DoubleCommand_create(NULL, addr, val.toInt(), false, 0);
        break;
    case C_SE_NB_1: sc = (InformationObject)SetpointCommandScaled_create(NULL, addr, val.toInt(), false, 0);
        break;
    case C_SE_NC_1: sc = (InformationObject)SetpointCommandShort_create(NULL, addr, val.toFloat(), false, 0);
        break;
    case C_BO_NA_1: sc = (InformationObject)Bitstring32Command_create(NULL, addr, val.toUInt());
        break;
    default: sc = nullptr;
    }

    if(sc != nullptr)
    {
        commandQueue.enqueue(sc);
    }

}

void ConnectIEC101Thread::disconnect()
{
        SerialPort_destroy(port);
        if (master) emit closeConnection();
//        this->disconnect();
        this->terminate();
        this->deleteLater();
}

void ConnectIEC101Thread::run()
{

}
