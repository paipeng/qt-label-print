QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    pdfutil.cpp

HEADERS += \
    mainwindow.h \
    pdfutil.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    qt-label-print_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



INCLUDEPATH += $$PWD/../../C/libharu/include
INCLUDEPATH += $$PWD/../../C/libharu/build_mac/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../C/libharu/build_mac/src -lhpdf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../qt-smartcard/debug/ -lqt-smartcard
else:unix: LIBS += -L$$PWD/../../C/libharu/build_mac/src -lhpdf
