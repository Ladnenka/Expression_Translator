#include "../AbstractTranslator.h"

AbstractTranslator::TranslateContext::~TranslateContext() {}

AbstractTranslator::AbstractTranslator(TranslateContext* ctx) : context(ctx) {
    context->translator = this;
}

AbstractTranslator::~AbstractTranslator() { delete context; }

AbstractTranslator::TranslateContext::LoadedData::LoadedData() {}

QString AbstractTranslator::translateExpression(ExprNode* root) {

    preLines.clear();
    postLines.clear();

    QString mainResult = translate(root);

    return (preLines + QStringList{mainResult} + postLines).join("\n");
}

QString AbstractTranslator::translate(ExprNode* expr) {
    return QString();
}
