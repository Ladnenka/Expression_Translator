#ifndef ENGLISHTRANSLATOR_H
#define ENGLISHTRANSLATOR_H
#include "AbstractTranslator.h"


class EnglishTranslator : public AbstractTranslator {
public:
    class EnglishTranslateContext : public TranslateContext {
    public:

        EnglishTranslateContext(const TranslateContext::LoadedData& data);

        QString getVarDescription(const QString& varName) override;
        QString getFuncDescription(const QString& funcName, const QVector<ExprNode*>& operands) override;
    };

    EnglishTranslator(EnglishTranslateContext* ctx);

private:
    bool needParentheses(ExprNode::ExprType exprType, ExprNode::ExprType operandType) override;

    QString translateSum(const QVector<QString>& parts) override;
    QString translateSub(const QVector<QString>& parts) override;
    QString translateMultiply(const QVector<QString>& parts) override;
    QString translateDivide(const QVector<QString>& parts) override;
    QString translateModulo(const QVector<QString>& parts) override;
    QString translatePower(const QVector<QString>& parts) override;
    QString translateUnaryMinus(const QVector<QString>& parts) override;
    QString translateLogicalAnd(const QVector<QString>& parts) override;
    QString translateLogicalOr(const QVector<QString>& parts) override;
    QString translateLogicalNot(ExprNode* expr, const QVector<QString>& parts) override;
    QString translateLess(const QVector<QString>& parts) override;
    QString translateGreater(const QVector<QString>& parts) override;
    QString translateLessEq(const QVector<QString>& parts) override;
    QString translateGreaterEq(const QVector<QString>& parts) override;
    QString translateEqual(const QVector<QString>& parts) override;
    QString translateNotEqual(const QVector<QString>& parts) override;
    QString translateAssign(const QVector<QString>& parts) override;
    QString translatePlusAssign(const QVector<QString>& parts) override;
    QString translateMinusAssign(const QVector<QString>& parts) override;
    QString translateMultAssign(const QVector<QString>& parts) override;
    QString translateDivAssign(const QVector<QString>& parts) override;
    QString translateModAssign(const QVector<QString>& parts) override;
    QString translatePreInc(const QVector<QString>& parts) override;
    QString translatePostInc(const QVector<QString>& parts) override;
    QString translatePreDec(const QVector<QString>& parts) override;
    QString translatePostDec(const QVector<QString>& parts) override;
    QString translateAddressOf(const QVector<QString>& parts) override;
    QString translateDereference(ExprNode* expr, const QVector<QString>& parts) override;
    QString translateIndex(const QVector<QString>& parts) override;
};

#endif // ENGLISHTRANSLATOR_H
