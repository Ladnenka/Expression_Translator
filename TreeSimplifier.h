#ifndef TREESIMPLIFIER_H
#define TREESIMPLIFIER_H
#include "ExprNode.h"

class TreeSimplifier {
public:
    static void simplify(ExprNode*& root);

private:
    static ExprNode* makeNegative(ExprNode* op);

    static void simplifyPlus(ExprNode* root);
    static void simplifyMinus(ExprNode* root);
    static void simplifyMultiply(ExprNode* root);
    static void simplifyDivide(ExprNode* root);
    static void simplifyPointer(ExprNode*& root);
    static void simplifyUnaryMinus(ExprNode*& root);
};

#endif // TREESIMPLIFIER_H
