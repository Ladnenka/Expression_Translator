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

        virtual QString getVarType(const QString& varName) = 0;
        virtual QString getVarDescription(const QString& varName) = 0;
        virtual QString getFuncDescription(const QString& funcName, const QVector<ExprNode*>& operands) = 0;

        AbstractTranslator* translator = nullptr;

    protected:
        LoadedData loadedData;
    };

protected:
    TranslateContext* context;
    QStringList preLines;
    QStringList postLines;

    virtual bool needParentheses(ExprNode::ExprType exprType, ExprNode::ExprType operandType);

    QString translateNumber(ExprNode* expr);

    virtual QString translateSum(const QVector<QString>& parts) = 0;
    virtual QString translateSub(const QVector<QString>& parts) = 0;
    virtual QString translateMultiply(const QVector<QString>& parts) = 0;
    virtual QString translateDivide(const QVector<QString>& parts) = 0;
    virtual QString translateModulo(const QVector<QString>& parts) = 0;
    virtual QString translatePower(const QVector<QString>& parts) = 0;
    virtual QString translateUnaryMinus(const QVector<QString>& parts) = 0;
    virtual QString translateLogicalAnd(const QVector<QString>& parts) = 0;
    virtual QString translateLogicalOr(const QVector<QString>& parts) = 0;
    virtual QString translateLogicalNot(ExprNode* expr, const QVector<QString>& parts) = 0;
    virtual QString translateLess(const QVector<QString>& parts) = 0;
    virtual QString translateGreater(const QVector<QString>& parts) = 0;
    virtual QString translateLessEq(const QVector<QString>& parts) = 0;
    virtual QString translateGreaterEq(const QVector<QString>& parts) = 0;
    virtual QString translateEqual(const QVector<QString>& parts) = 0;
    virtual QString translateNotEqual(const QVector<QString>& parts) = 0;
    virtual QString translateAssign(const QVector<QString>& parts) = 0;
    virtual QString translatePlusAssign(const QVector<QString>& parts) = 0;
    virtual QString translateMinusAssign(const QVector<QString>& parts) = 0;
    virtual QString translateMultAssign(const QVector<QString>& parts) = 0;
    virtual QString translateDivAssign(const QVector<QString>& parts) = 0;
    virtual QString translateModAssign(const QVector<QString>& parts) = 0;
    virtual QString translatePreInc(const QVector<QString>& parts) = 0;
    virtual QString translatePostInc(const QVector<QString>& parts) = 0;
    virtual QString translatePreDec(const QVector<QString>& parts) = 0;
    virtual QString translatePostDec(const QVector<QString>& parts) = 0;
    virtual QString translateAddressOf(const QVector<QString>& parts) = 0;
    virtual QString translateDereference(ExprNode* expr, const QVector<QString>& parts) = 0;
    virtual QString translateIndex(const QVector<QString>& parts) = 0;

public:
    AbstractTranslator(TranslateContext* ctx);
    virtual ~AbstractTranslator();
    QString translateExpression(ExprNode* root);
    QString translate(ExprNode* expr);
};

#endif // ABSTRACTTRANSLATOR_H
