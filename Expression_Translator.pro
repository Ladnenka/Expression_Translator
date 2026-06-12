QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MAIN.cpp \
    models/Variable.cpp \
    models/Function.cpp \
    models/Error.cpp \
    core/ExprNode.cpp \
    core/TreeBuilder.cpp \
    core/TreeSimplifier.cpp \
    translator/AbstractTranslator.cpp \
    translator/EnglishTranslator.cpp \
    Functions_IO.cpp \
    JsonDataLoader.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    AbstractTranslator.h \
    EnglishTranslator.h \
    Error.h \
    ExprNode.h \
    Function.h \
    Functions_IO.h \
    JsonDataLoader.h \
    TreeBuilder.h \
    TreeSimplifier.h \
    Variable.h
