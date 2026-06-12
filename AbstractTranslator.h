#ifndef ABSTRACTTRANSLATOR_H
#define ABSTRACTTRANSLATOR_H
#include "Variable.h"
#include "Function.h"
#include <QHash>
#include <QString>
#include <QStringList>

class AbstractTranslator
{
public:
public:
    class TranslateContext {
    public:
        struct LoadedData {
            QList<Variable> variables;
            QList<Function> functions;
            QStringList functionNames;
            QHash<QString, int> functionArgCount;

            LoadedData();
            LoadedData(const QList<Variable>& vars, const QList<Function>& funcs);
        };

        TranslateContext(const LoadedData& data) : loadedData(data) {}


        AbstractTranslator* translator = nullptr;

    protected:
        LoadedData loadedData;
    };

protected:
    TranslateContext* context;
    QStringList preLines;
    QStringList postLines;

public:
    AbstractTranslator(TranslateContext* ctx);

};

#endif // ABSTRACTTRANSLATOR_H
