#include "../ExprNode.h"

ExprNode::ExprNode() {}

ExprNode::ExprNode(ExprType t) : type(t) {}

ExprNode::ExprNode(ExprType t, const QString& val) : type(t), value(val) {}

ExprNode::ExprNode(double num) : type(NUMBER), value(QString::number(num)) {}

ExprNode::ExprNode(const QString& var) : type(VARIABLE), varName(var) {}

ExprNode::ExprNode(ExprType t, const QString& name, const QVector<ExprNode*>& args)
    : type(t), funcName(name), operands(args) {}

ExprNode* ExprNode::makeErrorNode() {
    return new ExprNode(ERROR_NODE, QString("?"));
}

ExprNode::~ExprNode() {
    for (ExprNode* child : operands) delete child;
}
