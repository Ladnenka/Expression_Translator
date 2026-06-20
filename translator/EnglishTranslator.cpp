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
