#include <QtTest>
#include <QObject>
#include "../TreeSimplifier.h"
#include "../ExprNode.h"

class TEST_SimplifyPlus : public QObject {
    Q_OBJECT
public:
    TEST_SimplifyPlus() {}
    ~TEST_SimplifyPlus() {}

private slots:
    void Test1_SimplePlus();
    void Test2_ChainLeft();
    void Test3_ChainRight();
    void Test4_ChainThree();
    void Test5_MinusLeft();
    void Test6_MinusRight();
    void Test7_MinusBoth();
    void Test8_VariableOperand();
    void Test9_MultiplyOperand();
    void Test10_NestedTwoMinus();
    void Test11_SingleOperand();
    void Test12_TwoUnaryMinus();
    void Test13_SameVariable();
    void Test14_PrefixIncrement();
    void Test15_TwoMultiplyOperands();
    void Test16_MinusAndUnaryMinus();
};

//Test 1: Plus(Var(a), Var(b)) → Plus(Var(a), Var(b))
void TEST_SimplifyPlus::Test1_SimplePlus() {
    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    root->operands.append(a);
    root->operands.append(b);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("b"));

    delete root;
}

//Test 2: Plus(Plus(Var(a), Var(b)), Var(c)) → Plus(Var(a), Var(b), Var(c))
void TEST_SimplifyPlus::Test2_ChainLeft() {
    ExprNode* inner = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    inner->operands.append(a);
    inner->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    root->operands.append(inner);
    root->operands.append(c);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 3);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("b"));
    QCOMPARE(root->operands[2]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[2]->varName, QString("c"));

    delete root;
}

//Test 3: Plus(Var(a), Plus(Var(b), Var(c))) → Plus(Var(a), Var(b), Var(c))
void TEST_SimplifyPlus::Test3_ChainRight() {
    ExprNode* inner = new ExprNode(ExprNode::PLUS);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    inner->operands.append(b);
    inner->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    root->operands.append(a);
    root->operands.append(inner);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 3);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("b"));
    QCOMPARE(root->operands[2]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[2]->varName, QString("c"));

    delete root;
}

//Test 4: Plus(Plus(Plus(Var(a), Var(b)), Var(c)), Var(d)) → Plus(Var(a), Var(b), Var(c), Var(d))
void TEST_SimplifyPlus::Test4_ChainThree() {
    ExprNode* inner1 = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    inner1->operands.append(a);
    inner1->operands.append(b);

    ExprNode* inner2 = new ExprNode(ExprNode::PLUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    inner2->operands.append(inner1);
    inner2->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    root->operands.append(inner2);
    root->operands.append(d);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 4);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->varName, QString("b"));
    QCOMPARE(root->operands[2]->varName, QString("c"));
    QCOMPARE(root->operands[3]->varName, QString("d"));

    delete root;
}

//Test 5: Plus(Minus(Var(a), Var(b)), Var(c)) → Plus(Var(a), UnaryMinus(Var(b)), Var(c))
void TEST_SimplifyPlus::Test5_MinusLeft() {
    ExprNode* minus = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    minus->operands.append(a);
    minus->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    root->operands.append(minus);
    root->operands.append(c);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 3);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::UNARY_MINUS);
    QVERIFY(!root->operands[1]->operands.isEmpty());
    QCOMPARE(root->operands[1]->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("b"));
    QCOMPARE(root->operands[2]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[2]->varName, QString("c"));

    delete root;
}

//Test 6: Plus(Var(a), Minus(Var(b), Var(c))) → Plus(Var(a), Var(b), UnaryMinus(Var(c)))
void TEST_SimplifyPlus::Test6_MinusRight() {
    ExprNode* minus = new ExprNode(ExprNode::MINUS);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    minus->operands.append(b);
    minus->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    root->operands.append(a);
    root->operands.append(minus);

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

//Test 7: Plus(Minus(Var(a), Var(b)), Minus(Var(c), Var(d))) → Plus(Var(a), UnaryMinus(Var(b)), Var(c), UnaryMinus(Var(d)))
void TEST_SimplifyPlus::Test7_MinusBoth() {
    ExprNode* minus1 = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    minus1->operands.append(a);
    minus1->operands.append(b);

    ExprNode* minus2 = new ExprNode(ExprNode::MINUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    minus2->operands.append(c);
    minus2->operands.append(d);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    root->operands.append(minus1);
    root->operands.append(minus2);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 4);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::UNARY_MINUS);
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("b"));
    QCOMPARE(root->operands[2]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[2]->varName, QString("c"));
    QCOMPARE(root->operands[3]->type, ExprNode::UNARY_MINUS);
    QCOMPARE(root->operands[3]->operands[0]->varName, QString("d"));

    delete root;
}

//Test 8: Plus(Var(a), Function(f)) → Plus(Var(a), Function(f))
void TEST_SimplifyPlus::Test8_VariableOperand() {
    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* f = new ExprNode(ExprNode::FUNCTION); f->funcName = "f";
    root->operands.append(a);
    root->operands.append(f);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->funcName, QString("f"));

    delete root;
}

//Test 9: Plus(Var(a), Multiply(Var(b), Var(c))) → Plus(Var(a), Multiply(Var(b), Var(c)))
void TEST_SimplifyPlus::Test9_MultiplyOperand() {
    ExprNode* mul = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    mul->operands.append(b);
    mul->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    root->operands.append(a);
    root->operands.append(mul);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::MULTIPLY);

    delete root;
}

//Test 10: Plus(Plus(Minus(Var(a),Var(b)), Minus(Var(c),Var(d))), Var(e))
//        → Plus(Var(a), UnaryMinus(Var(b)), Var(c), UnaryMinus(Var(d)), Var(e))
void TEST_SimplifyPlus::Test10_NestedTwoMinus() {
    ExprNode* minus1 = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    minus1->operands.append(a);
    minus1->operands.append(b);

    ExprNode* minus2 = new ExprNode(ExprNode::MINUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    minus2->operands.append(c);
    minus2->operands.append(d);

    ExprNode* inner = new ExprNode(ExprNode::PLUS);
    inner->operands.append(minus1);
    inner->operands.append(minus2);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* e = new ExprNode(ExprNode::VARIABLE); e->varName = "e";
    root->operands.append(inner);
    root->operands.append(e);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 5);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::UNARY_MINUS);
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("b"));
    QCOMPARE(root->operands[2]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[2]->varName, QString("c"));
    QCOMPARE(root->operands[3]->type, ExprNode::UNARY_MINUS);
    QCOMPARE(root->operands[3]->operands[0]->varName, QString("d"));
    QCOMPARE(root->operands[4]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[4]->varName, QString("e"));

    delete root;
}

//Test 11: одиночный Var(a) → тип VARIABLE
void TEST_SimplifyPlus::Test11_SingleOperand() {
    ExprNode* root = new ExprNode(ExprNode::VARIABLE);
    root->varName = "a";

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands.size(), 0);

    delete root;
}

//Test 12: Plus(UnaryMinus(Var(a)), UnaryMinus(Var(b)))
void TEST_SimplifyPlus::Test12_TwoUnaryMinus() {
    ExprNode* um1 = new ExprNode(ExprNode::UNARY_MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    um1->operands.append(a);

    ExprNode* um2 = new ExprNode(ExprNode::UNARY_MINUS);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    um2->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    root->operands.append(um1);
    root->operands.append(um2);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::UNARY_MINUS);
    QVERIFY(!root->operands[0]->operands.isEmpty());
    QCOMPARE(root->operands[0]->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::UNARY_MINUS);
    QVERIFY(!root->operands[1]->operands.isEmpty());
    QCOMPARE(root->operands[1]->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("b"));

    delete root;
}

//Test 13: Plus(Plus(Var(a), Var(a)), Var(a)) → Plus(Var(a), Var(a), Var(a))
void TEST_SimplifyPlus::Test13_SameVariable() {
    ExprNode* inner = new ExprNode(ExprNode::PLUS);
    ExprNode* a1 = new ExprNode(ExprNode::VARIABLE); a1->varName = "a";
    ExprNode* a2 = new ExprNode(ExprNode::VARIABLE); a2->varName = "a";
    inner->operands.append(a1);
    inner->operands.append(a2);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* a3 = new ExprNode(ExprNode::VARIABLE); a3->varName = "a";
    root->operands.append(inner);
    root->operands.append(a3);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 3);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->varName, QString("a"));
    QCOMPARE(root->operands[2]->varName, QString("a"));

    delete root;
}

//Test 14: Plus(Plus(Var(a), PreInc(Var(b))), Var(c)) → Plus(Var(a), PreInc(Var(b)), Var(c))
void TEST_SimplifyPlus::Test14_PrefixIncrement() {
    ExprNode* preInc = new ExprNode(ExprNode::PRE_INC);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    preInc->operands.append(b);

    ExprNode* inner = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    inner->operands.append(a);
    inner->operands.append(preInc);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    root->operands.append(inner);
    root->operands.append(c);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 3);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::PRE_INC);
    QVERIFY(!root->operands[1]->operands.isEmpty());
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("b"));
    QCOMPARE(root->operands[2]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[2]->varName, QString("c"));

    delete root;
}

//Test 15: Plus(Plus(Var(a), Mul(Var(b),Var(c))), Mul(Var(d),Var(e)))
//        → Plus(Var(a), Mul(Var(b),Var(c)), Mul(Var(d),Var(e)))
void TEST_SimplifyPlus::Test15_TwoMultiplyOperands() {
    ExprNode* mul1 = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    mul1->operands.append(b);
    mul1->operands.append(c);

    ExprNode* inner = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    inner->operands.append(a);
    inner->operands.append(mul1);

    ExprNode* mul2 = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* d = new ExprNode(ExprNode::VARIABLE); d->varName = "d";
    ExprNode* ee = new ExprNode(ExprNode::VARIABLE); ee->varName = "e";
    mul2->operands.append(d);
    mul2->operands.append(ee);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    root->operands.append(inner);
    root->operands.append(mul2);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 3);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::MULTIPLY);
    QCOMPARE(root->operands[2]->type, ExprNode::MULTIPLY);

    delete root;
}

//Test 16: Plus(Minus(Var(a), Var(b)), UnaryMinus(Var(c)))
//        → Plus(Var(a), UnaryMinus(Var(b)), UnaryMinus(Var(c)))
void TEST_SimplifyPlus::Test16_MinusAndUnaryMinus() {
    ExprNode* minus = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    minus->operands.append(a);
    minus->operands.append(b);

    ExprNode* um = new ExprNode(ExprNode::UNARY_MINUS);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    um->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    root->operands.append(minus);
    root->operands.append(um);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 3);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::UNARY_MINUS);
    QCOMPARE(root->operands[1]->operands[0]->varName, QString("b"));
    QCOMPARE(root->operands[2]->type, ExprNode::UNARY_MINUS);
    QCOMPARE(root->operands[2]->operands[0]->varName, QString("c"));

    delete root;
}

#include "TEST_SimplifyPlus.moc"
