QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
INCLUDEPATH += \
    lib/duckx/include \
    lib/duckx/include/pugixml \
    lib/duckx/include/zip \

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/docxbuffer.cpp \
    src/mainwindow.cpp \
    src/wordlist_stringarray.cpp \
    src/txtoutput_wordlist.cpp \
# docx lib - DuckX
    lib/duckx/src/duckx.cpp \
    lib/duckx/src/pugixml.cpp \
    lib/duckx/src/zip.c

HEADERS += \
    header/docxbuffer.h \
    header/mainwindow.h \
    header/wordlist_stringarray.h \
    header/txtoutput_wordlist.h \
# declaration only
    header/wordlist_base.h \
    header/define.h

FORMS += \
    script/mainwindow.ui

TRANSLATIONS += \
    script/langpack_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
