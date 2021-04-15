QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    iec60870/buffer_frame.c \
    iec60870/cpXXtime2a.c \
    iec60870/cs101_asdu.c \
    iec60870/cs101_bcr.c \
    iec60870/cs101_information_objects.c \
    iec60870/cs101_master.c \
    iec60870/cs101_master_connection.c \
    iec60870/cs101_queue.c \
    iec60870/cs101_slave.c \
    iec60870/cs104_connection.c \
    iec60870/cs104_frame.c \
    iec60870/frame.c \
    iec60870/lib60870_common.c \
    iec60870/lib_memory.c \
    iec60870/link_layer.c \
    iec60870/linked_list.c \
    iec60870/serial_port_win32.c \
    iec60870/serial_transceiver_ft_1_2.c \
    iec60870/socket_win32.c \
    iec60870/thread_win32.c \
    iec60870/time.c \
    main.cpp \
    mainwindow.cpp

LIBS += -lws2_32

HEADERS += \
    iec60870/apl_types_internal.h \
    iec60870/buffer_frame.h \
    iec60870/cs101_asdu_internal.h \
    iec60870/cs101_information_objects.h \
    iec60870/cs101_master.h \
    iec60870/cs101_queue.h \
    iec60870/cs101_slave.h \
    iec60870/cs104_connection.h \
    iec60870/cs104_frame.h \
    iec60870/frame.h \
    iec60870/hal_serial.h \
    iec60870/hal_socket.h \
    iec60870/hal_thread.h \
    iec60870/hal_time.h \
    iec60870/iec60870_common.h \
    iec60870/iec60870_master.h \
    iec60870/iec60870_slave.h \
    iec60870/information_objects_internal.h \
    iec60870/lib60870_config.h \
    iec60870/lib60870_internal.h \
    iec60870/lib_memory.h \
    iec60870/link_layer.h \
    iec60870/link_layer_parameters.h \
    iec60870/link_layer_private.h \
    iec60870/linked_list.h \
    iec60870/platform_endian.h \
    iec60870/serial_transceiver_ft_1_2.h \
    iec60870/tls_config.h \
    iec60870/tls_socket.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
