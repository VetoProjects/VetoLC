#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T13:06:15
#
#-------------------------------------------------

QT       += core gui multimedia testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VetoLCTest
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -Wall -DNDEBUG -O2 -pedantic

with_python2{
    QMAKE_CXXFLAGS += `/usr/bin/python2.7-config --cflags`
    QMAKE_LFLAGS += `/usr/bin/python2.7-config --ldflags`
    LIBS += -lpython2.7
}
with_python{
    QMAKE_CXXFLAGS += `/usr/bin/python-config --cflags`
    QMAKE_LFLAGS += `/usr/bin/python-config --ldflags`
}
with_python_custom || with_python2 || with_python{
    SOURCES += ../src/PySoundGenerator.cpp\
    ../src/PyLiveInterpreter.cpp

    HEADERS  += PySoundGeneratorTest.hpp \
    ../src/PySoundGenerator.hpp \
    PyLiveTest.hpp \
    ../src/PyLiveInterpreter.hpp

}

RESOURCES += \
    ../application.qrc

HEADERS += \
    EditorWindowTest.hpp \
    CodeEditorTest.hpp \
    ../src/SettingsWindow.hpp \
    ../src/SettingsTab.hpp \
    ../src/Renderer.hpp \
    ../src/CodeEditor.hpp \
    ../src/CodeHighlighter.hpp \
    ../src/EditorWindow.hpp \
    ../src/SoundGenerator.hpp \
    SoundGeneratorTest.hpp \
    ../src/Backend.hpp \
    ../src/LiveThread.hpp \
    BackendTest.hpp \
    ../src/SettingsBackend.hpp \
    ../src/AudioInputProcessor.hpp \
    ../src/BootLoader.hpp \
    ../src/CodeEditorTest.hpp \
    ../src/Instances/IInstance.hpp \
    SettingsBackendTest.hpp \
    AudioInputProcessorTest.hpp \
    RendererTest.hpp \
    ../src/AudioOutputProcessor.hpp \
    ../src/Instances/WindowInstance.hpp \
    AudioOutputProcessorTest.hpp \
    CodeHighlighterTest.hpp

SOURCES += \
    ../src/CodeEditor.cpp \
    ../src/CodeHighlighter.cpp \
    ../src/EditorWindow.cpp \
    ../src/SoundGenerator.cpp \
    ../src/SettingsWindow.cpp \
    ../src/SettingsTab.cpp \
    ../src/Renderer.cpp \
    main.cpp \
    ../src/Backend.cpp \
    ../src/SettingsBackend.cpp \
    ../src/AudioInputProcessor.cpp \
    ../src/BootLoader.cpp \
    ../src/Instances/WindowInstance.cpp \
    ../src/AudioOutputProcessor.cpp
