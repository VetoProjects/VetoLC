#-------------------------------------------------
#
# Project created by QtCreator 2014-04-24T10:20:34
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VeToLC
TEMPLATE = app
win32:RC_ICONS += ../rc/Icon.ico
ICON = ../rc/Icon.icns

TRANSLATIONS = ../translations/codeeditor_de.ts \
               ../translations/codeeditor_en.ts \
               ../translations/codeeditor_fr.ts

CODECFORSRC     = UTF-8

QMAKE_CXXFLAGS += -Wall -DNDEBUG -O2 -pedantic
QMAKE_EXTRA_TARGETS += valgrind-check

CONFIG+=c++11 no_keywords

valgrind-check.depends = check
valgrind-check.commands = valgrind ./$$TARGET

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

    SOURCES += \
    PySoundGenerator.cpp\
    PyLiveInterpreter.cpp

    HEADERS  += \
    PySoundGenerator.hpp\
    PyLiveInterpreter.hpp

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

HEADERS += Instances/IInstance.hpp \
    Instances/WindowInstance.hpp \
    AudioInputProcessor.hpp \
    Backend.hpp \
    BootLoader.hpp \
    CodeEditor.hpp \
    CodeHighlighter.hpp \
    EditorWindow.hpp \
    LiveThread.hpp \
    Renderer.hpp \
    SettingsBackend.hpp \
    SettingsTab.hpp \
    SettingsWindow.hpp \
    SoundGenerator.hpp \
    AudioOutputProcessor.hpp

SOURCES += Instances/WindowInstance.cpp \
    AudioInputProcessor.cpp \
    Backend.cpp \
    BootLoader.cpp \
    CodeEditor.cpp \
    CodeHighlighter.cpp \
    EditorWindow.cpp \
    main.cpp \
    Renderer.cpp \
    SettingsBackend.cpp \
    SettingsTab.cpp \
    SettingsWindow.cpp \
    SoundGenerator.cpp \
    AudioOutputProcessor.cpp
