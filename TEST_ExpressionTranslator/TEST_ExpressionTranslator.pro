QT += testlib
QT -= gui
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
TEMPLATE = app

SOURCES += \
    main.cpp \
    Testutils.cpp \
    TEST_SimplifyPlus.cpp \
    TEST_SimplifyMinus.cpp \
    TEST_SimplifyDivide.cpp \
    TEST_GetFunctionDescription.cpp \
    $$PWD/../core/ExprNode.cpp \
    $$PWD/../core/TreeSimplifier.cpp \
    $$PWD/../translator/AbstractTranslator.cpp \
    $$PWD/../translator/EnglishTranslator.cpp \
    $$PWD/../models/Variable.cpp \
    $$PWD/../models/Function.cpp

HEADERS += \
    TestUtils.h

INCLUDEPATH += $$PWD/..
