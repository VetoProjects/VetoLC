#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T13:06:15
#
#-------------------------------------------------

QT       += core gui multimedia opengl testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LiveCodingTest
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11 -Wall -Werror -DNDEBUG -O2 -pedantic

with_python{
    QMAKE_CXXFLAGS += `/usr/bin/python2.7-config --cflags`
    QMAKE_LFLAGS += `/usr/bin/python2.7-config --ldflags`
    LIBS += -lpython2.7
}with_python_custom || with_python{
    SOURCES += ../PySoundGenerator.cpp

    HEADERS  += PySoundGeneratorTest.hpp \
    ../PySoundGenerator.hpp

}

RESOURCES += \
    ../application.qrc

HEADERS += \
    EditorWindowTest.hpp \
    CodeEditorTest.hpp \
    ../PythonException.hpp \
    ../QtSoundException.hpp \
    ../SettingsWindow.hpp \
    ../SettingsTab.hpp \
    ../Renderer.hpp \
    ../CodeEditor.hpp \
    ../CodeHighlighter.hpp \
    ../EditorWindow.hpp \
    ../LiveCodingSetup.hpp \
    ../SoundGenerator.hpp \
    CodeHighlighterTest.hpp \
    SettingsTabTest.hpp \
    SettingsWindowTest.hpp \
    SoundGeneratorTest.hpp \
    ../Backend.hpp \
    ../LiveThread.hpp \
    BackendTest.hpp \
    ../SettingsBackend.hpp \
    ../AudioInputProcessor.hpp \
    ../BootLoader.hpp \
    ../CodeEditorTest.hpp \
    ../QtGlException.hpp \
    ../Instances/IInstance.hpp \
    ../Instances/WindowInstance.hpp

SOURCES += \
    ../CodeEditor.cpp \
    ../CodeHighlighter.cpp \
    ../EditorWindow.cpp \
    ../LiveCodingSetup.cpp \
    ../SoundGenerator.cpp \
    ../SettingsWindow.cpp \
    ../SettingsTab.cpp \
    ../Renderer.cpp \
    main.cpp \
    ../Backend.cpp \
    ../SettingsBackend.cpp \
    ../AudioInputProcessor.cpp \
    ../BootLoader.cpp \
    ../Instances/WindowInstance.cpp
