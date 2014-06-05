#-------------------------------------------------
#
# Project created by QtCreator 2014-04-24T10:20:34
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LiveCoding
TEMPLATE = app

TRANSLATIONS = codeeditor_de.ts \
               codeeditor_en.ts \
               codeeditor_fr.ts

CODECFORSRC     = UTF-8

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CFLAGS += -Wall -Werror -DNDEBUG -O2 -pedantic
QMAKE_EXTRA_TARGETS += valgrind-check

CONFIG+=c++11

valgrind-check.depends = check
valgrind-check.commands = valgrind ./$$TARGET

with_python{

    QMAKE_CXXFLAGS += `/usr/bin/python2.7-config --cflags`
    QMAKE_LFLAGS += `/usr/bin/python2.7-config --ldflags`
    LIBS += -lpython2.7

}
with_python_custom || with_python{

    SOURCES += \
    PySoundGenerator.cpp\
    PyLiveInterpreter.cpp

    HEADERS  += \
    PySoundGenerator.hpp\
    PyLiveInterpreter.hpp

}

RESOURCES += \
    application.qrc

HEADERS += CodeEditor.hpp \
    CodeHighlighter.hpp \
    EditorWindow.hpp \
    LiveCodingSetup.hpp \
    SoundGenerator.hpp\
    PythonException.hpp \
    QtSoundException.hpp \
    SettingsWindow.hpp \
    SettingsTab.hpp \
    Renderer.hpp \
    SettingsBackend.hpp \
    Backend.hpp \
    QtGlException.hpp \
    LiveThread.hpp \
    BootLoader.hpp \
    AudioInputProcessor.hpp \
    Instances/IInstance.hpp \
    Instances/WindowInstance.hpp

SOURCES += main.cpp\
    CodeEditor.cpp \
    CodeHighlighter.cpp \
    EditorWindow.cpp \
    LiveCodingSetup.cpp \
    SoundGenerator.cpp \
    SettingsTab.cpp \
    SettingsWindow.cpp \
    Renderer.cpp \
    SettingsBackend.cpp \
    Backend.cpp \
    BootLoader.cpp \
    AudioInputProcessor.cpp \
    Instances/WindowInstance.cpp
