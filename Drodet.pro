QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller/detectioncontroller.cpp \
    detection/yolodetector.cpp \
    main.cpp \
    mainwindow.cpp \
    sources/imagesource.cpp \
    sources/videofilesource.cpp \
    utils/frameprocessor.cpp

HEADERS += \
    controller/detectioncontroller.h \
    detection/detectionresult.h \
    detection/yolodetector.h \
    mainwindow.h \
    sources/iframesource.h \
    sources/imagesource.h \
    sources/videofilesource.h \
    utils/frameprocessor.h

FORMS += \
    mainwindow.ui

unix:!macx {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv4
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
