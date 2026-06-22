#include "../EnglishTranslator.h"

EnglishTranslator::EnglishTranslateContext::EnglishTranslateContext(const AbstractTranslator::TranslateContext::LoadedData& data)
    : TranslateContext(data) {}

EnglishTranslator::EnglishTranslator(EnglishTranslateContext* ctx)
    : AbstractTranslator(ctx) {}

QString EnglishTranslator::EnglishTranslateContext::getFuncDescription(
    const QString& funcName, const QVector<ExprNode*>& operands)
{
    return QString();
}

QString EnglishTranslator::EnglishTranslateContext::getVarDescription(const QString& varName) {
    return QString();
}

bool EnglishTranslator::needParentheses(ExprNode::ExprType exprType, ExprNode::ExprType operandType) { return false; }

QString EnglishTranslator::translateSum(const QVector<QString>& parts)                              { return QString(); }
QString EnglishTranslator::translateSub(const QVector<QString>& parts)                              { return QString(); }
QString EnglishTranslator::translateMultiply(const QVector<QString>& parts)                         { return QString(); }
QString EnglishTranslator::translateDivide(const QVector<QString>& parts)                           { return QString(); }
QString EnglishTranslator::translateModulo(const QVector<QString>& parts)                           { return QString(); }
QString EnglishTranslator::translatePower(const QVector<QString>& parts)                            { return QString(); }
QString EnglishTranslator::translateUnaryMinus(const QVector<QString>& parts)                       { return QString(); }
QString EnglishTranslator::translateLogicalAnd(const QVector<QString>& parts)                       { return QString(); }
QString EnglishTranslator::translateLogicalOr(const QVector<QString>& parts)                        { return QString(); }
QString EnglishTranslator::translateLogicalNot(ExprNode* expr, const QVector<QString>& parts)       { return QString(); }
QString EnglishTranslator::translateLess(const QVector<QString>& parts)                             { return QString(); }
QString EnglishTranslator::translateGreater(const QVector<QString>& parts)                          { return QString(); }
QString EnglishTranslator::translateLessEq(const QVector<QString>& parts)                           { return QString(); }
QString EnglishTranslator::translateGreaterEq(const QVector<QString>& parts)                        { return QString(); }
QString EnglishTranslator::translateEqual(const QVector<QString>& parts)                            { return QString(); }
QString EnglishTranslator::translateNotEqual(const QVector<QString>& parts)                         { return QString(); }
QString EnglishTranslator::translateAssign(const QVector<QString>& parts)                           { return QString(); }
QString EnglishTranslator::translatePlusAssign(const QVector<QString>& parts)                       { return QString(); }
QString EnglishTranslator::translateMinusAssign(const QVector<QString>& parts)                      { return QString(); }
QString EnglishTranslator::translateMultAssign(const QVector<QString>& parts)                       { return QString(); }
QString EnglishTranslator::translateDivAssign(const QVector<QString>& parts)                        { return QString(); }
QString EnglishTranslator::translateModAssign(const QVector<QString>& parts)                        { return QString(); }
QString EnglishTranslator::translatePreInc(const QVector<QString>& parts)                           { return QString(); }
QString EnglishTranslator::translatePostInc(const QVector<QString>& parts)                          { return QString(); }
QString EnglishTranslator::translatePreDec(const QVector<QString>& parts)                           { return QString(); }
QString EnglishTranslator::translatePostDec(const QVector<QString>& parts)                          { return QString(); }
QString EnglishTranslator::translateAddressOf(const QVector<QString>& parts)                        { return QString(); }
QString EnglishTranslator::translateDereference(ExprNode* expr, const QVector<QString>& parts)      { return QString(); }
QString EnglishTranslator::translateIndex(const QVector<QString>& parts)                            { return QString(); }
