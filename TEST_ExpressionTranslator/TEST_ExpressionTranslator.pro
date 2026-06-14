QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    TEST_SimplifyDivide.cpp \
    TEST_SimplifyMinus.cpp \
    TEST_SimplifyPlus.cpp \
    ../core/exprnode.cpp \
    ../core/treesimplifier.cpp \
    main.cpp


