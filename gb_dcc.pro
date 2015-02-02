#
# QT interface to Gertbot runnign as DCC controller
#

CONFIG += qt debug thread
TEMPLATE = app
INCLUDEPATH +=  .

HEADERS += gertbot_defines.h

SOURCES += main.cpp
HEADERS += gb_dcc.h
SOURCES += gb_dcc.cpp
FORMS   += gb_dcc.ui
RESOURCES += gb_dcc.qrc

win32 {
     HEADERS += gertbot_winserial.h
     SOURCES += gertbot_winserial.c
     # need this lib for the wprintf routine in gertbot_winserial.c     
     LIBS += User32.lib
 } else {
     HEADERS += gertbot_pi_uart.h
     SOURCES += gertbot_pi_uart.c
 }
 
