#include <QtTest>
#include <QObject>
#include "../TreeSimplifier.h"
#include "../ExprNode.h"

class TEST_SimplifyMinus : public QObject {
    Q_OBJECT
public:
    TEST_SimplifyMinus() {}
    ~TEST_SimplifyMinus() {}

private slots:
    void Test1_NotMinusType();
    void Test2_VarVarUnchanged();
    void Test3_LeftMultiplyUnchanged();
    void Test4_RightMultiplyUnchanged();
    void Test5_RightUnaryMinusUnchanged();
    void Test6_BranchA_TwoTerms();
    void Test7_BranchA_ThreeTerms();
    void Test8_BranchA_RightMultiply();
    void Test9_BranchB_Simple();
    void Test10_RightPostDecrementUnchanged();
    void Test11_BranchB_PlusSimplified();
    void Test12_BranchB_Recursive();
    void Test13_BranchC_Simple();
    void Test14_BranchC_RightMultiply();
    void Test15_BranchB_SimplifyPlusExpandsMinus();
    void Test16_Complex_BranchA_RightMinus();
    void Test17_BranchA_DoubleNegation();
};

//Test 1: Var(a) — не MINUS, ничего не меняется
void TEST_SimplifyMinus::Test1_NotMinusType() {
    ExprNode* root = new ExprNode(ExprNode::VARIABLE);
    root->varName = "a";

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands.size(), 0);

    delete root;
}

//Test 2: Minus(Var(a), Var(b)) — ничего не меняется
void TEST_SimplifyMinus::Test2_VarVarUnchanged() {
    ExprNode* root = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    root->operands.append(a);
    root->operands.append(b);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::MINUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("b"));

    delete root;
}

//Test 3: Minus(Multiply(Var(a),Var(b)), Var(c)) — ничего не меняется
void TEST_SimplifyMinus::Test3_LeftMultiplyUnchanged() {
    ExprNode* mul = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    mul->operands.append(a);
    mul->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    root->operands.append(mul);
    root->operands.append(c);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::MINUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("c"));

    delete root;
}

//Test 4: Minus(Var(a), Multiply(Var(b),Var(c))) — ничего не меняется
void TEST_SimplifyMinus::Test4_RightMultiplyUnchanged() {
    ExprNode* mul = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    mul->operands.append(b);
    mul->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    root->operands.append(a);
    root->operands.append(mul);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::MINUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::MULTIPLY);

    delete root;
}

//Test 5: Minus(Var(a), UnaryMinus(Var(b))) — ничего не меняется
void TEST_SimplifyMinus::Test5_RightUnaryMinusUnchanged() {
    ExprNode* um = new ExprNode(ExprNode::UNARY_MINUS);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    um->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    root->operands.append(a);
    root->operands.append(um);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::MINUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::UNARY_MINUS);
    QVERIFY(!root->operands[1]->operands.isEmpty());
    QCOMPARE(root->operands[1]->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("b"));

    delete root;
}

//Test 6: Minus(Plus(Var(a),Var(b)), Var(c)) → Plus(Var(a), Var(b), UnaryMinus(Var(c)))
void TEST_SimplifyMinus::Test6_BranchA_TwoTerms() {
    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    plus->operands.append(a);
    plus->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    root->operands.append(plus);
    root->operands.append(c);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 3);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("b"));
    QCOMPARE(root->operands[2]->type, ExprNode::UNARY_MINUS);
    QVERIFY(!root->operands[2]->operands.isEmpty());
    QCOMPARE(root->operands[2]->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[2]->operands[0]->varName, QString("c"));

    delete root;
}

//Test 7: Minus(Plus(Var(a),Var(b),Var(c)), Var(d)) → Plus(Var(a), Var(b), Var(c), UnaryMinus(Var(d)))
void TEST_SimplifyMinus::Test7_BranchA_ThreeTerms() {
    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    plus->operands.append(a);
    plus->operands.append(b);
    plus->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    root->operands.append(plus);
    root->operands.append(d);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 4);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->varName, QString("b"));
    QCOMPARE(root->operands[2]->varName, QString("c"));
    QCOMPARE(root->operands[3]->type, ExprNode::UNARY_MINUS);
    QCOMPARE(root->operands[3]->operands[0]->varName, QString("d"));

    delete root;
}

//Test 8: Minus(Plus(Var(a),Var(b)), Multiply(Var(c),Var(d)))
//       → Plus(Var(a), Var(b), UnaryMinus(Multiply(Var(c),Var(d))))
void TEST_SimplifyMinus::Test8_BranchA_RightMultiply() {
    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    plus->operands.append(a);
    plus->operands.append(b);

    ExprNode* mul = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    mul->operands.append(c);
    mul->operands.append(d);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    root->operands.append(plus);
    root->operands.append(mul);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 3);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->varName, QString("b"));
    QCOMPARE(root->operands[2]->type, ExprNode::UNARY_MINUS);
    QVERIFY(!root->operands[2]->operands.isEmpty());
    QCOMPARE(root->operands[2]->operands[0]->type, ExprNode::MULTIPLY);

    delete root;
}

//Test 9: Minus(Minus(Var(a),Var(b)), Var(c)) → Minus(Var(a), Plus(Var(b),Var(c)))
void TEST_SimplifyMinus::Test9_BranchB_Simple() {
    ExprNode* inner = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    inner->operands.append(a);
    inner->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    root->operands.append(inner);
    root->operands.append(c);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::MINUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::PLUS);

    delete root;
}

//Test 10: Minus(Var(a), PostDec(Var(b)))- ничего не меняется
void TEST_SimplifyMinus::Test10_RightPostDecrementUnchanged() {
    ExprNode* postDec = new ExprNode(ExprNode::POST_DEC);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    postDec->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    root->operands.append(a);
    root->operands.append(postDec);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::MINUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::POST_DEC);
    QVERIFY(!root->operands[1]->operands.isEmpty());
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("b"));

    delete root;
}

//Test 11: Minus(Minus(Var(a),Var(b)), Plus(Var(c),Var(d)))
//        → Minus(Var(a), Plus(Var(b),Var(c),Var(d)))
void TEST_SimplifyMinus::Test11_BranchB_PlusSimplified() {
    ExprNode* inner = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    inner->operands.append(a);
    inner->operands.append(b);

    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    plus->operands.append(c);
    plus->operands.append(d);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    root->operands.append(inner);
    root->operands.append(plus);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::MINUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::PLUS);
    QCOMPARE(root->operands[1]->operands.size(), 3);

    delete root;
}

//Test 12: Minus(Minus(Minus(Var(a),Var(b)),Var(c)), Var(d))
//        → Minus(Var(a), Plus(Var(b),Var(c),Var(d)))
void TEST_SimplifyMinus::Test12_BranchB_Recursive() {
    ExprNode* inner1 = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    inner1->operands.append(a);
    inner1->operands.append(b);

    ExprNode* inner2 = new ExprNode(ExprNode::MINUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    inner2->operands.append(inner1);
    inner2->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    root->operands.append(inner2);
    root->operands.append(d);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::MINUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::PLUS);
    QCOMPARE(root->operands[1]->operands.size(), 3);

    delete root;
}

//Test 13: Minus(Var(a), Minus(Var(b),Var(c))) → Minus(Plus(Var(a),Var(c)), Var(b))
void TEST_SimplifyMinus::Test13_BranchC_Simple() {
    ExprNode* inner = new ExprNode(ExprNode::MINUS);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    inner->operands.append(b);
    inner->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    root->operands.append(a);
    root->operands.append(inner);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::MINUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::PLUS);
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("b"));

    delete root;
}

//Test 14: Minus(Var(a), Minus(Var(b), Multiply(Var(c),Var(d))))
//        → Minus(Plus(Var(a), Multiply(Var(c),Var(d))), Var(b))
void TEST_SimplifyMinus::Test14_BranchC_RightMultiply() {
    ExprNode* mul = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    mul->operands.append(c);
    mul->operands.append(d);

    ExprNode* inner = new ExprNode(ExprNode::MINUS);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    inner->operands.append(b);
    inner->operands.append(mul);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    root->operands.append(a);
    root->operands.append(inner);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::MINUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::PLUS);
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("b"));

    delete root;
}

//Test 15: Minus(Minus(Var(a),Var(b)), Minus(Var(c),Var(d)))
//        → Minus(Var(a), Plus(Var(b),Var(c),UnaryMinus(Var(d))))
void TEST_SimplifyMinus::Test15_BranchB_SimplifyPlusExpandsMinus() {
    ExprNode* inner1 = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    inner1->operands.append(a);
    inner1->operands.append(b);

    ExprNode* inner2 = new ExprNode(ExprNode::MINUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    inner2->operands.append(c);
    inner2->operands.append(d);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    root->operands.append(inner1);
    root->operands.append(inner2);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::MINUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::PLUS);
    QCOMPARE(root->operands[1]->operands.size(), 3);

    delete root;
}

//Test 16: Minus(Plus(Var(a),Var(b)), Minus(Var(c),Var(d)))
//        → Plus(Var(a), Var(b), UnaryMinus(Minus(Var(c),Var(d))))
void TEST_SimplifyMinus::Test16_Complex_BranchA_RightMinus() {
    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    plus->operands.append(a);
    plus->operands.append(b);

    ExprNode* inner = new ExprNode(ExprNode::MINUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    inner->operands.append(c);
    inner->operands.append(d);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    root->operands.append(plus);
    root->operands.append(inner);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 3);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->varName, QString("b"));
    QCOMPARE(root->operands[2]->type, ExprNode::UNARY_MINUS);
    QVERIFY(!root->operands[2]->operands.isEmpty());
    QCOMPARE(root->operands[2]->operands[0]->type, ExprNode::MINUS);

    delete root;
}

//Test 17: Minus(Plus(Var(a),Var(b)), UnaryMinus(Var(c)))
//        → Plus(Var(a), Var(b), Var(c))
void TEST_SimplifyMinus::Test17_BranchA_DoubleNegation() {
    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    plus->operands.append(a);
    plus->operands.append(b);

    ExprNode* um = new ExprNode(ExprNode::UNARY_MINUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    um->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    root->operands.append(plus);
    root->operands.append(um);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 3);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->varName, QString("b"));
    QCOMPARE(root->operands[2]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[2]->varName, QString("c"));

    delete root;
}

#include "TEST_SimplifyMinus.moc"
