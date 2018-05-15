#---------------------------------------------------------------------------
#  Copyright 2016 ESRI
#
#  All rights reserved under the copyright laws of the United States
#  and applicable international laws, treaties, and conventions.
#
#  You may freely redistribute and use this sample code, with or
#  without modification, provided you include the original copyright
#  notice and use restrictions.
#
#  See the Sample code usage restrictions document for further information.
#---------------------------------------------------------------------------

TARGET = fds_sismo 
TEMPLATE = app
QT += positioning core gui opengl network widgets sensors script sql
CONFIG += c++11

equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 9) { 
        error("$$TARGET requires Qt 5.9.2")
    }

    equals(QT_MINOR_VERSION, 9) : lessThan(QT_PATCH_VERSION, 2){
        error("$$TARGET requires Qt 5.9.2")
    }
}

ARCGIS_RUNTIME_VERSION = 100.2
include($$PWD/src/arcgisruntime.pri)

win32:CONFIG += \
    embed_manifest_exe

SOURCES += \
    src\main.cpp \
    src/fds_sismo.cpp \
    src/dao/networkdao.cpp \
    src/model/network.cpp \
    src/model/station.cpp \
    src/util/filemgr.cpp \
    src/util/unetwork.cpp \
    src/windows/data_viz/datawidget.cpp \
    src/windows/data_viz/plotsettings.cpp \
    src/windows/data_viz/qplotter.cpp \
    src/windows/data_viz/stationconfig.cpp \
    src/windows/map/MMap.cpp \
    src/windows/eventloc.cpp \
    src/windows/stationviz.cpp \
    src/windows/svm.cpp \
    src/windows/mdialog.cpp \
    src/windows/imgdialog.cpp \
    src/windows/stsubmit.cpp

HEADERS += \
    src/fds_sismo.h \
    src/dao/networkdao.h \
    src/model/network.h \
    src/model/station.h \
    src/util/filemgr.h \
    src/util/unetwork.h \
    src/windows/data_viz/datawidget.h \
    src/windows/data_viz/plotsettings.h \
    src/windows/data_viz/plotter.h \
    src/windows/data_viz/stationconfig.h \
    src/windows/map/MMap.h \
    src/windows/eventloc.h \
    src/windows/stationviz.h \
    src/windows/svm.h \
    src/global/global.h \
    src/windows/mdialog.h \
    src/model/event.h \
    src/windows/imgdialog.h \
    ext/json.hpp \
    src/windows/stsubmit.h

#-------------------------------------------------------------------------------

DISTFILES += \
    src/arcgisruntime.pri \
    src/windows/data_viz/data/img/zoomin.png \
    src/windows/data_viz/data/img/zoomout.png \
    src/windows/data_viz/data/stations_data/station.data

RESOURCES += \
    src/img.qrc \
    src/windows/data_viz/data/qplotter.qrc

FORMS += \
    src/mainwindow.ui \
     src/windows/imgdialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -lqtcsv
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -lqtcsv
else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lqtcsv

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

 QMAKE_LFLAGS += -Wl,-rpath,"'$$ORIGIN'"
