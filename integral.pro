TEMPLATE = app
CONFIG += qt \

CONFIG(debug, debug|release) { 
    DESTDIR = debug_bin
    win32:CONFIG += console
    OBJECTS_DIR = debug
}
else: { 
    DESTDIR = release_bin
    OBJECTS_DIR = release
}
DEPENDPATH += . \
    src \
    src/ui
INCLUDEPATH += . \
    src
MOC_DIR = moc/
UI_DIR = src/ui
FORMS += mainwindow.ui
HEADERS += src/integequ.h \
    mainwindow.h

RESOURCES += src/resources.qrc
SOURCES += src/integequ.cpp \
    mainwindow.cpp \
	main.cpp

TRANSLATIONS += src/translations/integral_ua.ts