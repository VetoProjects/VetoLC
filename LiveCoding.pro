#-------------------------------------------------
#
# Project created by QtCreator 2014-04-24T10:20:34
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LiveCoding
TEMPLATE = app

TRANSLATIONS = translations/codeeditor_de.ts \
               translations/codeeditor_en.ts \
               translations/codeeditor_fr.ts

CODECFORSRC     = UTF-8

QMAKE_CXXFLAGS += -Wall -DNDEBUG -O2 -pedantic
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
    src/PySoundGenerator.cpp\
    src/PyLiveInterpreter.cpp

    HEADERS  += \
    src/PySoundGenerator.hpp\
    src/PyLiveInterpreter.hpp

}

RESOURCES += \
    application.qrc

HEADERS += src/Instances/IInstance.hpp \
    src/Instances/WindowInstance.hpp \
    src/AudioInputProcessor.hpp \
    src/Backend.hpp \
    src/BootLoader.hpp \
    src/CodeEditor.hpp \
    src/CodeHighlighter.hpp \
    src/EditorWindow.hpp \
    src/LiveThread.hpp \
    src/Renderer.hpp \
    src/SettingsBackend.hpp \
    src/SettingsTab.hpp \
    src/SettingsWindow.hpp \
    src/SoundGenerator.hpp \
    src/AudioOutputProcessor.hpp

SOURCES += src/Instances/WindowInstance.cpp \
    src/AudioInputProcessor.cpp \
    src/Backend.cpp \
    src/BootLoader.cpp \
    src/CodeEditor.cpp \
    src/CodeHighlighter.cpp \
    src/EditorWindow.cpp \
    src/main.cpp \
    src/Renderer.cpp \
    src/SettingsBackend.cpp \
    src/SettingsTab.cpp \
    src/SettingsWindow.cpp \
    src/SoundGenerator.cpp \
    src/AudioOutputProcessor.cpp
