#include "connectIEC101thread.h"

ConnectIEC101Thread::ConnectIEC101Thread(QString port): serialPort(port)
{

}

bool ConnectIEC101Thread::isConnect()
{
    if(master)
    {
        if(running)
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
    signal(SIGINT, sigint_handler);

    port = SerialPort_create(serialPort.toStdString().c_str(), 9600, 8, 'E', 1);

    CS101_Master master = CS101_Master_create(port, NULL, NULL, IEC60870_LINK_LAYER_UNBALANCED);

    /* Setting the callback handler for generic ASDUs */
    CS101_Master_setASDUReceivedHandler(master, asduReceivedHandler, NULL);

    /* set callback handler for link layer state changes */
    CS101_Master_setLinkLayerStateChanged(master, linkLayerStateChanged, NULL);

    CS101_Master_addSlave(master, 1);
    CS101_Master_addSlave(master, 2);

    SerialPort_open(port);

    running = true;

    while(1)
    {
        if(!commandQueue.isEmpty())
        {
            if (CS101_Master_isChannelReady(master, 1))
            {
                InformationObject sc = commandQueue.dequeue();
                CS101_Master_useSlaveAddress(master, 1);
                CS101_Master_sendProcessCommand(master, CS101_COT_ACTIVATION, 1, sc);
                CS101_Master_run(master);

                InformationObject_destroy(sc);
            }
        }
    }
}
