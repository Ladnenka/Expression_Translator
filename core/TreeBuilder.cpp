#include "../TreeBuilder.h"

ExprNode* TreeBuilder::buildTree(const QStringList& tokens, const QString& originalExpr,
                                 const AbstractTranslator::TranslateContext::LoadedData& data,
                                 QList<Error>& errors) {
    return new ExprNode();
}

TreeBuilder::~TreeBuilder() {
}
