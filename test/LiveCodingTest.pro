#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T13:06:15
#
#-------------------------------------------------

QT       += core gui multimedia testlib widgets

isEqual(QT_MAJOR_VERSION, 4) {
    message("Cannot build VetoLC with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.2.")
}

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

translations.target = $$OUT_PWD/translations
src = $$PWD/../translations
win32 {
    translations.target ~= s,/,\\,g
    src ~= s,/,\\,g
}
translations.commands = $(COPY_DIR) \"$$src\" \"$$translations.target\"
first.depends = $(first) translations
export(first.depends)
export(translations.commands)

QMAKE_EXTRA_TARGETS += first translations

HEADERS += \
    EditorWindowTest.hpp \
    ../src/EditorWindow.hpp \
    CodeEditorTest.hpp \
    ../src/CodeEditor.hpp \
    SoundGeneratorTest.hpp \
    ../src/SoundGenerator.hpp \
    BackendTest.hpp \
    ../src/Backend.hpp \
    SettingsBackendTest.hpp \
    ../src/SettingsBackend.hpp \
    AudioInputProcessorTest.hpp \
    ../src/AudioInputProcessor.hpp \
    RendererTest.hpp \
    ../src/Instances/WindowInstance.hpp \
    AudioOutputProcessorTest.hpp \
    ../src/AudioOutputProcessor.hpp \
    CodeHighlighterTest.hpp \
    ../src/SettingsWindow.hpp \
    ../src/SettingsTab.hpp \
    ../src/Renderer.hpp \
    ../src/CodeHighlighter.hpp \
    ../src/LiveThread.hpp \
    ../src/BootLoader.hpp \
    ../src/Instances/IInstance.hpp

SOURCES += \
    main.cpp \
    ../src/CodeEditor.cpp \
    ../src/CodeHighlighter.cpp \
    ../src/EditorWindow.cpp \
    ../src/SoundGenerator.cpp \
    ../src/SettingsWindow.cpp \
    ../src/SettingsTab.cpp \
    ../src/Renderer.cpp \
    ../src/Backend.cpp \
    ../src/SettingsBackend.cpp \
    ../src/AudioInputProcessor.cpp \
    ../src/BootLoader.cpp \
    ../src/Instances/WindowInstance.cpp \
    ../src/AudioOutputProcessor.cpp
