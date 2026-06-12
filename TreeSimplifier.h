#ifndef TREESIMPLIFIER_H
#define TREESIMPLIFIER_H
#include "ExprNode.h"

class TreeSimplifier {
public:
    static void simplify(ExprNode*& root);

private:

    static void simplifyPlus(ExprNode* root);
};

#endif // TREESIMPLIFIER_H
