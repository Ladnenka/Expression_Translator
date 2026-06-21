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

    bool isNumberToken(const QString& token);

    bool buildBinaryOp(ExprNode::ExprType opType,int position, QList<Error>& errors);
    bool buildUnaryOp(ExprNode::ExprType opType, int position, QList<Error>& errors);
    bool buildNAryOp(ExprNode::ExprType opType, int position, QList<Error>& errors);
    bool buildFunctionCall(int position,const AbstractTranslator::TranslateContext::LoadedData& data, QList<Error>& errors);
    ExprNode* buildTree(const QStringList& tokens,
                        const QString& originalExpr,
                        const AbstractTranslator::TranslateContext::LoadedData& data,
                        QList<Error>& errors);

    ~TreeBuilder();

private:
    bool checkSideEffect(ExprNode* operand, int position, QList<Error>& errors);
};

#endif // TREEBUILDER_H
