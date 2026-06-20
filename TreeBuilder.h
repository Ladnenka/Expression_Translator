#ifndef TREEBUILDER_H
#define TREEBUILDER_H
#include "ExprNode.h"
#include <QStack>
#include <QSet>
#include "AbstractTranslator.h"
#include "Error.h"

class TreeBuilder {
public:
    QStack<ExprNode*> stack;
    QSet<QString> modifiedVariables;

    ExprNode* buildTree(const QStringList& tokens,
                        const QString& originalExpr,
                        const AbstractTranslator::TranslateContext::LoadedData& data,
                        QList<Error>& errors);

    ~TreeBuilder();

};

#endif // TREEBUILDER_H
