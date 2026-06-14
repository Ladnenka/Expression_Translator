#include <QtTest>
#include <QObject>
#include "../TreeSimplifier.h"
#include "../ExprNode.h"

class TEST_SimplifyDivide : public QObject {
    Q_OBJECT
public:
    TEST_SimplifyDivide() {}
    ~TEST_SimplifyDivide() {}

private slots:
    void Test1_SimpleDivide();
    void Test2_LeftDivide();
    void Test3_RightDivide();
    void Test4_ChainThreeDivides();
    void Test5_LeftMultiplyUnchanged();
    void Test6_RightPlusUnchanged();
    void Test7_LeftPlusUnchanged();
    void Test8_BothDivide();
    void Test9_NestedRightThenLeft();
    void Test10_LeftMultiplyThenDivide();
    void Test11_ChainFourDivides();
    void Test12_NotDivideType();
    void Test13_BranchA_RightMultiply();
    void Test14_BranchB_LeftMultiply();
};

//Test 1: Divide(Var(a), Var(b)) → без изменений
void TEST_SimplifyDivide::Test1_SimpleDivide() {
    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    root->operands.append(a);
    root->operands.append(b);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::DIVIDE);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("b"));

    delete root;
}

//Test 2: Divide(Divide(Var(a),Var(b)), Var(c)) → Divide(Var(a), Multiply(Var(b),Var(c)))
void TEST_SimplifyDivide::Test2_LeftDivide() {
    ExprNode* inner = new ExprNode(ExprNode::DIVIDE);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    inner->operands.append(a);
    inner->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    root->operands.append(inner);
    root->operands.append(c);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::DIVIDE);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[1]->operands.size(), 2);
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("b"));
    QCOMPARE(root->operands[1]->operands[1]->varName, QString("c"));

    delete root;
}

//Test 3: Divide(Var(a), Divide(Var(b),Var(c))) → Divide(Multiply(Var(a),Var(c)), Var(b))
void TEST_SimplifyDivide::Test3_RightDivide() {
    ExprNode* inner = new ExprNode(ExprNode::DIVIDE);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    inner->operands.append(b);
    inner->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    root->operands.append(a);
    root->operands.append(inner);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::DIVIDE);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[0]->operands.size(), 2);
    QCOMPARE(root->operands[0]->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[0]->operands[1]->varName, QString("c"));
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("b"));

    delete root;
}

//Test 4: Divide(Divide(Divide(Var(a),Var(b)),Var(c)), Var(d))
//       → Divide(Var(a), Multiply(Var(b),Var(c),Var(d)))
void TEST_SimplifyDivide::Test4_ChainThreeDivides() {
    ExprNode* inner1 = new ExprNode(ExprNode::DIVIDE);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    inner1->operands.append(a);
    inner1->operands.append(b);

    ExprNode* inner2 = new ExprNode(ExprNode::DIVIDE);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    inner2->operands.append(inner1);
    inner2->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    root->operands.append(inner2);
    root->operands.append(d);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::DIVIDE);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[1]->operands.size(), 3);
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("b"));
    QCOMPARE(root->operands[1]->operands[1]->varName, QString("c"));
    QCOMPARE(root->operands[1]->operands[2]->varName, QString("d"));

    delete root;
}

//Test 5: Divide(Multiply(Var(a),Var(b)), Var(c)) → без изменений
void TEST_SimplifyDivide::Test5_LeftMultiplyUnchanged() {
    ExprNode* mul = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    mul->operands.append(a);
    mul->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    root->operands.append(mul);
    root->operands.append(c);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::DIVIDE);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("c"));

    delete root;
}

//Test 6: Divide(Var(a), Plus(Var(b),Var(c))) → без изменений
void TEST_SimplifyDivide::Test6_RightPlusUnchanged() {
    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    plus->operands.append(b);
    plus->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    root->operands.append(a);
    root->operands.append(plus);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::DIVIDE);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::PLUS);

    delete root;
}

//Test 7: Divide(Plus(Var(a),Var(b)), Var(c)) → без изменений
void TEST_SimplifyDivide::Test7_LeftPlusUnchanged() {
    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    plus->operands.append(a);
    plus->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    root->operands.append(plus);
    root->operands.append(c);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::DIVIDE);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::PLUS);
    QCOMPARE(root->operands[1]->varName, QString("c"));

    delete root;
}

//Test 8: Divide(Divide(Var(a),Var(b)), Divide(Var(c),Var(d)))
//        → Divide(a, Multiply(b, Divide(c,d)))
void TEST_SimplifyDivide::Test8_BothDivide() {
    ExprNode* left = new ExprNode(ExprNode::DIVIDE);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    left->operands.append(a);
    left->operands.append(b);

    ExprNode* right = new ExprNode(ExprNode::DIVIDE);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    right->operands.append(c);
    right->operands.append(d);

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    root->operands.append(left);
    root->operands.append(right);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::DIVIDE);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[1]->operands.size(), 2);
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("b"));
    QCOMPARE(root->operands[1]->operands[1]->type, ExprNode::DIVIDE);
    QCOMPARE(root->operands[1]->operands[1]->operands.size(), 2);
    QCOMPARE(root->operands[1]->operands[1]->operands[0]->varName, QString("c"));
    QCOMPARE(root->operands[1]->operands[1]->operands[1]->varName, QString("d"));

    delete root;
}

//Test 9: Divide(Divide(Var(a), Divide(Var(b),Var(c))), Var(d))
//       → Divide(Multiply(Var(a),Var(c)), Multiply(Var(b),Var(d)))
void TEST_SimplifyDivide::Test9_NestedRightThenLeft() {
    ExprNode* inner = new ExprNode(ExprNode::DIVIDE);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    inner->operands.append(b);
    inner->operands.append(c);

    ExprNode* mid = new ExprNode(ExprNode::DIVIDE);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    mid->operands.append(a);
    mid->operands.append(inner);

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    root->operands.append(mid);
    root->operands.append(d);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::DIVIDE);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[1]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[0]->operands.size(), 2);
    QCOMPARE(root->operands[0]->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[0]->operands[1]->varName, QString("c"));
    QCOMPARE(root->operands[1]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[1]->operands.size(), 2);
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("b"));
    QCOMPARE(root->operands[1]->operands[1]->varName, QString("d"));


    delete root;
}

//Test 10: Divide(Divide(Multiply(Var(a),Var(b)), Var(c)), Var(d))
//        → Divide(Multiply(Var(a),Var(b)), Multiply(Var(c),Var(d)))
void TEST_SimplifyDivide::Test10_LeftMultiplyThenDivide() {
    ExprNode* mul = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    mul->operands.append(a);
    mul->operands.append(b);

    ExprNode* inner = new ExprNode(ExprNode::DIVIDE);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    inner->operands.append(mul);
    inner->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    root->operands.append(inner);
    root->operands.append(d);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::DIVIDE);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[1]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[1]->operands.size(), 2);
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("c"));
    QCOMPARE(root->operands[1]->operands[1]->varName, QString("d"));

    delete root;
}

//Test 11: Divide(Divide(Divide(Divide(a,b),c),d), e) → Divide(a, Multiply(b,c,d,e))
void TEST_SimplifyDivide::Test11_ChainFourDivides() {
    ExprNode* d1 = new ExprNode(ExprNode::DIVIDE);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    d1->operands.append(a);
    d1->operands.append(b);

    ExprNode* d2 = new ExprNode(ExprNode::DIVIDE);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    d2->operands.append(d1);
    d2->operands.append(c);

    ExprNode* d3 = new ExprNode(ExprNode::DIVIDE);
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    d3->operands.append(d2);
    d3->operands.append(d);

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    ExprNode* e = new ExprNode(ExprNode::VARIABLE); e->varName = "e";
    root->operands.append(d3);
    root->operands.append(e);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::DIVIDE);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[1]->operands.size(), 4);
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("b"));
    QCOMPARE(root->operands[1]->operands[1]->varName, QString("c"));
    QCOMPARE(root->operands[1]->operands[2]->varName, QString("d"));
    QCOMPARE(root->operands[1]->operands[3]->varName, QString("e"));

    delete root;
}

//Test 12: Multiply(Var(a), Var(b)) → без изменений
void TEST_SimplifyDivide::Test12_NotDivideType() {
    ExprNode* root = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    root->operands.append(a);
    root->operands.append(b);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->varName, QString("b"));

    delete root;
}

//Test 13: Divide(Divide(Var(a),Var(b)), Multiply(Var(c),Var(d)))
//        → Divide(Var(a), Multiply(Var(b),Var(c),Var(d)))
void TEST_SimplifyDivide::Test13_BranchA_RightMultiply() {
    ExprNode* inner = new ExprNode(ExprNode::DIVIDE);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    inner->operands.append(a);
    inner->operands.append(b);

    ExprNode* mul = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    mul->operands.append(c);
    mul->operands.append(d);

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    root->operands.append(inner);
    root->operands.append(mul);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::DIVIDE);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[1]->operands.size(), 3);
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("b"));
    QCOMPARE(root->operands[1]->operands[1]->varName, QString("c"));
    QCOMPARE(root->operands[1]->operands[2]->varName, QString("d"));

    delete root;
}

//Test 14: Divide(Var(a), Divide(Multiply(Var(b),Var(c)), Var(d)))
//        → Divide(Multiply(Var(a),Var(b),Var(c)), Var(d))
void TEST_SimplifyDivide::Test14_BranchB_LeftMultiply() {
    ExprNode* mul = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    mul->operands.append(b);
    mul->operands.append(c);

    ExprNode* inner = new ExprNode(ExprNode::DIVIDE);
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    inner->operands.append(mul);
    inner->operands.append(d);

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    root->operands.append(a);
    root->operands.append(inner);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::DIVIDE);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[0]->operands.size(), 3);
    QCOMPARE(root->operands[0]->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[0]->operands[1]->varName, QString("b"));
    QCOMPARE(root->operands[0]->operands[2]->varName, QString("c"));
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("d"));

    delete root;
}

#include "TEST_SimplifyDivide.moc"
