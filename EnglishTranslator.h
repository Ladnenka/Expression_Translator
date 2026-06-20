#ifndef ENGLISHTRANSLATOR_H
#define ENGLISHTRANSLATOR_H
#include "AbstractTranslator.h"


class EnglishTranslator : public AbstractTranslator {
public:
    class EnglishTranslateContext : public TranslateContext {
    public:

        EnglishTranslateContext(const TranslateContext::LoadedData& data);

        QString getFuncDescription(const QString& funcName, const QVector<ExprNode*>& operands) override;
    };

    EnglishTranslator(EnglishTranslateContext* ctx);

private:

};

#endif // ENGLISHTRANSLATOR_H
