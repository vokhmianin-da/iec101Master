#include "connectIEC101thread.h"

void ConnectIEC101Thread::linkLayerStateChanged(void* parameter, int address, LinkLayerState state)
{
    printf("Link layer state changed for slave %i: ", address);

    switch (state) {
    case LL_STATE_IDLE:
        printf("IDLE\n");
        break;
    case LL_STATE_ERROR:
        printf("ERROR\n");
        break;
    case LL_STATE_BUSY:
        printf("BUSY\n");
        break;
    case LL_STATE_AVAILABLE:
        printf("AVAILABLE\n");
        break;
    }
}


bool ConnectIEC101Thread::asduReceivedHandler (void* parameter, int address, CS101_ASDU asdu)
{
    ConnectIEC101Thread *ptrConnectThread = static_cast<ConnectIEC101Thread *>(parameter); //берем указатель на объект соединения

        if (CS101_ASDU_getTypeID(asdu) == M_ME_TE_1) {  //число тип 35

            int i;

            for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

                MeasuredValueScaledWithCP56Time2a io =
                        (MeasuredValueScaledWithCP56Time2a) CS101_ASDU_getElement(asdu, i);

                if(ptrConnectThread->isConnect()) emit ptrConnectThread->getIEC101Info(InformationObject_getObjectAddress((InformationObject) io), MeasuredValueScaled_getValue((MeasuredValueScaled) io));

                MeasuredValueScaledWithCP56Time2a_destroy(io);
            }
        }
        else if (CS101_ASDU_getTypeID(asdu) == M_SP_NA_1) { //дискретное значение тип 1

            int i;

            for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

                SinglePointInformation io =
                        (SinglePointInformation) CS101_ASDU_getElement(asdu, i);

                if(ptrConnectThread->isConnect()) emit ptrConnectThread->getIEC101Info( InformationObject_getObjectAddress((InformationObject) io), MeasuredValueScaled_getValue((MeasuredValueScaled) io));


                SinglePointInformation_destroy(io);
            }
        }
        else if (CS101_ASDU_getTypeID(asdu) == C_TS_TA_1) {

        }
        else if (CS101_ASDU_getTypeID(asdu) == M_ME_TF_1) { //число с плавающей точкой тип 36

            int i;

            for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

                MeasuredValueScaledWithCP56Time2a io =
                        (MeasuredValueScaledWithCP56Time2a) CS101_ASDU_getElement(asdu, i);

                if(ptrConnectThread->isConnect()) emit ptrConnectThread->getIEC101Info(InformationObject_getObjectAddress((InformationObject) io), MeasuredValueScaled_getValue((MeasuredValueScaled) io));


                MeasuredValueScaledWithCP56Time2a_destroy(io);
            }
        }
        else if (CS101_ASDU_getTypeID(asdu) == M_BO_TB_1) { //строка тип 33

            int i;

            for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

                MeasuredValueScaledWithCP56Time2a io =
                        (MeasuredValueScaledWithCP56Time2a) CS101_ASDU_getElement(asdu, i);

                if(ptrConnectThread->isConnect()) emit ptrConnectThread->getIEC101Info(InformationObject_getObjectAddress((InformationObject) io), MeasuredValueScaled_getValue((MeasuredValueScaled) io));


                MeasuredValueScaledWithCP56Time2a_destroy(io);
            }
        }

        return true;
}
