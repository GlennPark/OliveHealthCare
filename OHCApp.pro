QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network sql

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chattingclient.cpp \
    chattingserver.cpp \
    customerdb.cpp \
    customerlist.cpp \
    customermanage.cpp \
    customersearch.cpp \
    logthread.cpp \
    main.cpp \
    mainwindow.cpp \
    merchandiselist.cpp \
    merchandisemanage.cpp \
    merchandisesearch.cpp \
    purchaselist.cpp \
    purchasemanage.cpp \
    purchasesearch.cpp

HEADERS += \
    chattingclient.h \
    chattingserver.h \
    customerdb.h \
    customerlist.h \
    customermanage.h \
    customersearch.h \
    logthread.h \
    mainwindow.h \
    merchandiselist.h \
    merchandisemanage.h \
    merchandisesearch.h \
    purchaselist.h \
    purchasemanage.h \
    purchasesearch.h

FORMS += \
    chattingserver.ui \
    customermanage.ui \
    customersearch.ui \
    mainwindow.ui \
    merchandisemanage.ui \
    merchandisesearch.ui \
    purchasemanage.ui \
    purchasesearch.ui

TRANSLATIONS += \
    OHCApp_ko_KR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
