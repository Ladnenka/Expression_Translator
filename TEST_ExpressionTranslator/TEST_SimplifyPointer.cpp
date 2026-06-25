#include <QtTest>
#include <QObject>
#include "../TreeSimplifier.h"
#include "../ExprNode.h"

class TEST_SimplifyPointer : public QObject {
    Q_OBJECT
public:
    TEST_SimplifyPointer() {}
    ~TEST_SimplifyPointer() {}

private slots:
    void Test1_DerefOfAddress();
    void Test2_AddressOfDeref();
    void Test3_DerefOfAddress_NestedInPlus();
    void Test4_AddressOfDeref_NestedInPlus();
    void Test5_DoubleDerefOfAddress();
    void Test6_DerefOfAddress_PlusInside();
    void Test7_AddressOfDeref_FunctionResult();
    void Test8_DerefOfAddress_NestedInMinus();
    void Test9_DerefOfAddress_InsideUnaryMinus();
    void Test10_DerefOfAddress_ChainedInPlus();
    void Test11_NoSimplify_DerefOnly();
};

//Test 1: Deref(AddressOf(Var(a))) → Var(a)
void TEST_SimplifyPointer::Test1_DerefOfAddress() {
    ExprNode* addr = new ExprNode(ExprNode::ADDRESS_OF);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    addr->operands.append(a);

    ExprNode* root = new ExprNode(ExprNode::DEREFERENCE);
    root->operands.append(addr);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::VARIABLE);
    QCOMPARE(root->varName, QString("a"));

    delete root;
}

//Test 2: AddressOf(Deref(Var(p))) → Var(p)
void TEST_SimplifyPointer::Test2_AddressOfDeref() {
    ExprNode* deref = new ExprNode(ExprNode::DEREFERENCE);
    ExprNode* p = new ExprNode(ExprNode::VARIABLE); p->varName = "p";
    deref->operands.append(p);

    ExprNode* root = new ExprNode(ExprNode::ADDRESS_OF);
    root->operands.append(deref);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::VARIABLE);
    QCOMPARE(root->varName, QString("p"));

    delete root;
}

//Test 3: Plus(Deref(AddressOf(Var(a))), Var(b)) → Plus(Var(a), Var(b))
void TEST_SimplifyPointer::Test3_DerefOfAddress_NestedInPlus() {
    ExprNode* addr = new ExprNode(ExprNode::ADDRESS_OF);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    addr->operands.append(a);

    ExprNode* deref = new ExprNode(ExprNode::DEREFERENCE);
    deref->operands.append(addr);

    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    root->operands.append(deref);
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

//Test 4: Plus(Var(a), AddressOf(Deref(Var(p)))) → Plus(Var(a), Var(p))
void TEST_SimplifyPointer::Test4_AddressOfDeref_NestedInPlus() {
    ExprNode* deref = new ExprNode(ExprNode::DEREFERENCE);
    ExprNode* p = new ExprNode(ExprNode::VARIABLE); p->varName = "p";
    deref->operands.append(p);

    ExprNode* addr = new ExprNode(ExprNode::ADDRESS_OF);
    addr->operands.append(deref);

    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    root->operands.append(a);
    root->operands.append(addr);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("p"));

    delete root;
}

//Test 5: Deref(AddressOf(Deref(AddressOf(Var(x))))) → Var(x)
void TEST_SimplifyPointer::Test5_DoubleDerefOfAddress() {
    ExprNode* addr1 = new ExprNode(ExprNode::ADDRESS_OF);
    ExprNode* x = new ExprNode(ExprNode::VARIABLE); x->varName = "x";
    addr1->operands.append(x);

    ExprNode* deref1 = new ExprNode(ExprNode::DEREFERENCE);
    deref1->operands.append(addr1);

    ExprNode* addr2 = new ExprNode(ExprNode::ADDRESS_OF);
    addr2->operands.append(deref1);

    ExprNode* root = new ExprNode(ExprNode::DEREFERENCE);
    root->operands.append(addr2);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::VARIABLE);
    QCOMPARE(root->varName, QString("x"));

    delete root;
}

//Test 6: Deref(AddressOf(Plus(Var(a), Var(b)))) → Plus(Var(a), Var(b))
void TEST_SimplifyPointer::Test6_DerefOfAddress_PlusInside() {
    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    plus->operands.append(a);
    plus->operands.append(b);

    ExprNode* addr = new ExprNode(ExprNode::ADDRESS_OF);
    addr->operands.append(plus);

    ExprNode* root = new ExprNode(ExprNode::DEREFERENCE);
    root->operands.append(addr);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("b"));

    delete root;
}

//Test 7: AddressOf(Deref(Function(f))) → Function(f)
void TEST_SimplifyPointer::Test7_AddressOfDeref_FunctionResult() {
    ExprNode* f = new ExprNode(ExprNode::FUNCTION); f->funcName = "f";

    ExprNode* deref = new ExprNode(ExprNode::DEREFERENCE);
    deref->operands.append(f);

    ExprNode* root = new ExprNode(ExprNode::ADDRESS_OF);
    root->operands.append(deref);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::FUNCTION);
    QCOMPARE(root->funcName, QString("f"));

    delete root;
}

//Test 8: Minus(Deref(AddressOf(Var(a))), Var(b)) → Minus(Var(a), Var(b))
void TEST_SimplifyPointer::Test8_DerefOfAddress_NestedInMinus() {
    ExprNode* addr = new ExprNode(ExprNode::ADDRESS_OF);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    addr->operands.append(a);

    ExprNode* deref = new ExprNode(ExprNode::DEREFERENCE);
    deref->operands.append(addr);

    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    root->operands.append(deref);
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

//Test 9: UnaryMinus(Deref(AddressOf(Var(a)))) → UnaryMinus(Var(a))
void TEST_SimplifyPointer::Test9_DerefOfAddress_InsideUnaryMinus() {
    ExprNode* addr = new ExprNode(ExprNode::ADDRESS_OF);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    addr->operands.append(a);

    ExprNode* deref = new ExprNode(ExprNode::DEREFERENCE);
    deref->operands.append(addr);

    ExprNode* root = new ExprNode(ExprNode::UNARY_MINUS);
    root->operands.append(deref);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::UNARY_MINUS);
    QCOMPARE(root->operands.size(), 1);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));

    delete root;
}

//Test 10: Plus(Deref(AddressOf(Var(a))), AddressOf(Deref(Var(p)))) → Plus(Var(a), Var(p))
void TEST_SimplifyPointer::Test10_DerefOfAddress_ChainedInPlus() {
    ExprNode* addr1 = new ExprNode(ExprNode::ADDRESS_OF);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    addr1->operands.append(a);

    ExprNode* deref1 = new ExprNode(ExprNode::DEREFERENCE);
    deref1->operands.append(addr1);

    ExprNode* deref2 = new ExprNode(ExprNode::DEREFERENCE);
    ExprNode* p = new ExprNode(ExprNode::VARIABLE); p->varName = "p";
    deref2->operands.append(p);

    ExprNode* addr2 = new ExprNode(ExprNode::ADDRESS_OF);
    addr2->operands.append(deref2);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    root->operands.append(deref1);
    root->operands.append(addr2);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::PLUS);
    QCOMPARE(root->operands.size(), 2);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("a"));
    QCOMPARE(root->operands[1]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[1]->varName, QString("p"));

    delete root;
}

//Test 11: Deref(Var(p)) → Deref(Var(p))
void TEST_SimplifyPointer::Test11_NoSimplify_DerefOnly() {
    ExprNode* p = new ExprNode(ExprNode::VARIABLE); p->varName = "p";

    ExprNode* root = new ExprNode(ExprNode::DEREFERENCE);
    root->operands.append(p);

    TreeSimplifier::simplify(root);

    QCOMPARE(root->type, ExprNode::DEREFERENCE);
    QCOMPARE(root->operands.size(), 1);
    QCOMPARE(root->operands[0]->type, ExprNode::VARIABLE);
    QCOMPARE(root->operands[0]->varName, QString("p"));

    delete root;
}

#include "TEST_SimplifyPointer.moc"
