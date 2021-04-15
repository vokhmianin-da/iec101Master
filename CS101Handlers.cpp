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
    printf("SLAVE %i: RECVD ASDU type: %s(%i) elements: %i\n",
           address,
           TypeID_toString(CS101_ASDU_getTypeID(asdu)),
           CS101_ASDU_getTypeID(asdu),
           CS101_ASDU_getNumberOfElements(asdu));

    if (CS101_ASDU_getTypeID(asdu) == M_ME_TE_1) {

        printf("  measured scaled values with CP56Time2a timestamp (M_ME_TE_1):\n");

        int i;

        for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

            MeasuredValueScaledWithCP56Time2a io =
                    (MeasuredValueScaledWithCP56Time2a) CS101_ASDU_getElement(asdu, i);

            if (io != NULL) {

                printf("    IOA: %i value: %i\n",
                       InformationObject_getObjectAddress((InformationObject) io),
                       MeasuredValueScaled_getValue((MeasuredValueScaled) io)
                       );

                MeasuredValueScaledWithCP56Time2a_destroy(io);
            }
            else {
                printf("     invalid object!\n");
            }
        }
    }
    else if (CS101_ASDU_getTypeID(asdu) == M_SP_NA_1) {
        printf("  single point information (M_SP_NA_1):\n");

        int i;

        for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

            SinglePointInformation io =
                    (SinglePointInformation) CS101_ASDU_getElement(asdu, i);


            if (io != NULL) {

                printf("    IOA: %i value: %i\n",
                       InformationObject_getObjectAddress((InformationObject) io),
                       SinglePointInformation_getValue((SinglePointInformation) io)
                       );

                SinglePointInformation_destroy(io);
            }
            else {
                printf("     invalid object!\n");
            }
        }
    }
    else if (CS101_ASDU_getTypeID(asdu) == M_EP_TD_1) {
        printf("   event of protection equipment (M_EP_TD_1):\n");

        int i;

        for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

            EventOfProtectionEquipmentWithCP56Time2a epe = (EventOfProtectionEquipmentWithCP56Time2a)
                    CS101_ASDU_getElement(asdu, i);

            if (epe != NULL) {

                SingleEvent singleEvent = EventOfProtectionEquipmentWithCP56Time2a_getEvent(epe);

                printf("    IOA: %i state: %i  QDQ: %i\n",
                       InformationObject_getObjectAddress((InformationObject) epe),
                       SingleEvent_getEventState(singleEvent),
                       SingleEvent_getQDP(singleEvent)
                       );

                EventOfProtectionEquipmentWithCP56Time2a_destroy(epe);
            }
            else {
                printf("     invalid object!\n");
            }
        }
    }

    return true;
}
