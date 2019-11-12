#-------------------------------------------------
#
# Project created by QtCreator 2019-10-06T03:44:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MIPS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    GUI/browsefile.cpp \
    GUI/data_mem_widget.cpp \
    GUI/editor.cpp \
    GUI/execute_widget.cpp \
    GUI/filestester.cpp \
    GUI/piperegwidget.cpp \
    GUI/regfile_widget.cpp \
    GUI/testcase.cpp \
    GUI/testwidget.cpp \
    GUI/tree_widget.cpp \
    Pipeline/execution.cpp \
    Pipeline/instructiondecode.cpp \
    Pipeline/instructionfetch.cpp \
    Pipeline/memorystage.cpp \
    Pipeline/path.cpp \
    Pipeline/scene.cpp \
    Pipeline/stage.cpp \
    Pipeline/writebackstage.cpp \
    data_memory.cpp \
        main.cpp \
        mainwindow.cpp \
        assembler.cpp \
        simulator.cpp \
        register_file.cpp \
        register.cpp \
        alu.cpp \
        data.cpp \
        gui.cpp \


HEADERS += \
    GUI/browsefile.h \
    GUI/data_mem_widget.h \
    GUI/editor.h \
    GUI/execute_widget.h \
    GUI/filestester.h \
    GUI/piperegwidget.h \
    GUI/regfile_widget.h \
    GUI/testcase.h \
    GUI/testwidget.h \
    GUI/tree_widget.h \
    Pipeline/execution.h \
    Pipeline/instructiondecode.h \
    Pipeline/instructionfetch.h \
    Pipeline/memorystage.h \
    Pipeline/path.h \
    Pipeline/scene.h \
    Pipeline/stage.h \
    Pipeline/writebackstage.h \
    data_memory.h \
        mainwindow.h \
        assembler.h \
        simulator.h \
        register_file.h \
        register.h \
        alu.h \
        data.h \
        gui.h \

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES += \
    GUI/warning-icon.png \
    dummy.txt
