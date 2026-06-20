#ifndef ABSTRACTTRANSLATOR_H
#define ABSTRACTTRANSLATOR_H
#include "Variable.h"
#include "Function.h"
#include <QHash>
#include <QString>
#include <QStringList>
#include "ExprNode.h"

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
        virtual ~TranslateContext();

        virtual QString getFuncDescription(const QString& funcName, const QVector<ExprNode*>& operands) = 0;

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
    virtual ~AbstractTranslator();
};

#endif // ABSTRACTTRANSLATOR_H
