#ifndef TREEBUILDER_H
#define TREEBUILDER_H
#include "ExprNode.h"
#include <QStack>
#include <QSet>

class TreeBuilder {
public:
    QStack<ExprNode*> stack;
    QSet<QString> modifiedVariables;


};

#endif // TREEBUILDER_H
