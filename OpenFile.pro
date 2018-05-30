#-------------------------------------------------
#
# Project created by QtCreator 2018-03-07T09:30:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenFile
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mypixmapitem.cpp \
    imageprocessor.cpp \
    setdialog.cpp \
    matdialog.cpp \
    imagefilter.cpp \
    gaussblurdialog.cpp \
    bilateraldialog.cpp \
    onlysizedialog.cpp \
    fft.cpp \
    matrix.cpp \
    inputmatrix.cpp

HEADERS  += mainwindow.h \
    mypixmapitem.h \
    imageprocessor.h \
    setdialog.h \
    matdialog.h \
    imagefilter.h \
    gaussblurdialog.h \
    bilateraldialog.h \
    onlysizedialog.h \
    fft.h \
    matrix.h \
    inputmatrix.h



FORMS    += mainwindow.ui \
    setdialog.ui \
    matdialog.ui \
    gaussblurdialog.ui \
    bilateraldialog.ui \
    onlysizedialog.ui \
    inputmatrix.ui
