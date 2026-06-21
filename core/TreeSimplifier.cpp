#include "../TreeSimplifier.h"

void TreeSimplifier::simplify(ExprNode*& root) {
    if (!root) return;

    for (int i = 0; i < root->operands.size(); i++)
        simplify(root->operands[i]);

    if (root->type == ExprNode::PLUS)
        simplifyPlus(root);
    else if (root->type == ExprNode::MINUS)
        simplifyMinus(root);
    else if (root->type == ExprNode::MULTIPLY)
        simplifyMultiply(root);
    else if (root->type == ExprNode::DIVIDE)
        simplifyDivide(root);
    else if (root->type == ExprNode::ADDRESS_OF || root->type == ExprNode::DEREFERENCE)
        simplifyPointer(root);
    else if (root->type == ExprNode::UNARY_MINUS)
        simplifyUnaryMinus(root);
}

void TreeSimplifier::simplifyPlus(ExprNode* root) {}

void TreeSimplifier::simplifyMinus(ExprNode* root) {}

void TreeSimplifier::simplifyMultiply(ExprNode* root) {}

void TreeSimplifier::simplifyDivide(ExprNode* root) {}

void TreeSimplifier::simplifyPointer(ExprNode*& root) {}

void TreeSimplifier::simplifyUnaryMinus(ExprNode*& root) {}
