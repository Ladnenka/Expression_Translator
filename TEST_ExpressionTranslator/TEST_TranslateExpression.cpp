#include <QtTest>
#include <QObject>
#include "TestUtils.h"
#include "../EnglishTranslator.h"
#include "../ExprNode.h"

class TEST_TranslateExpression : public QObject {
    Q_OBJECT
public:
    TEST_TranslateExpression() {}
    ~TEST_TranslateExpression() {}

private slots:
    void Test1_PreIncrement();
    void Test2_PostIncrement();
    void Test3_PreDecrement();
    void Test4_PostDecrement();
    void Test5_LogicalNot_LessEq();
    void Test6_LogicalNot_GreaterEq();
    void Test7_LogicalNot_Var();
    void Test8_LogicalNot_LogicalAnd();
    void Test9_PreInc_InsidePlus();
    void Test10_PostDec_InsideMinus();
    void Test11_LogicalOr_ThreeOperands();
};

//Test 1: translateExpression(PreInc(Var("i")))
void TEST_TranslateExpression::Test1_PreIncrement() {
    auto data = makeData({}, { Variable("i", "i", "int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* i = new ExprNode(ExprNode::VARIABLE); i->varName = "i";
    ExprNode* root = new ExprNode(ExprNode::PRE_INC);
    root->operands.append(i);

    QString result = translator->translateExpression(root);
    QCOMPARE(result, QString("Increment i\ni"));

    delete root;
    delete translator;
}

//Test 2: translateExpression(PostInc(Var("x")))
void TEST_TranslateExpression::Test2_PostIncrement() {
    auto data = makeData({}, { Variable("x", "x", "int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* x = new ExprNode(ExprNode::VARIABLE); x->varName = "x";
    ExprNode* root = new ExprNode(ExprNode::POST_INC);
    root->operands.append(x);

    QString result = translator->translateExpression(root);
    QCOMPARE(result, QString("x\nIncrement x"));

    delete root;
    delete translator;
}

//Test 3: translateExpression(PreDec(Var("n")))
void TEST_TranslateExpression::Test3_PreDecrement() {
    auto data = makeData({}, { Variable("n", "n", "int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* n = new ExprNode(ExprNode::VARIABLE); n->varName = "n";
    ExprNode* root = new ExprNode(ExprNode::PRE_DEC);
    root->operands.append(n);

    QString result = translator->translateExpression(root);
    QCOMPARE(result, QString("Decrement n\nn"));

    delete root;
    delete translator;
}

//Test 4: translateExpression(PostDec(Var("k")))
void TEST_TranslateExpression::Test4_PostDecrement() {
    auto data = makeData({}, { Variable("k", "k", "int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* k = new ExprNode(ExprNode::VARIABLE); k->varName = "k";
    ExprNode* root = new ExprNode(ExprNode::POST_DEC);
    root->operands.append(k);

    QString result = translator->translateExpression(root);
    QCOMPARE(result, QString("k\nDecrement k"));

    delete root;
    delete translator;
}

//Test 5: translateExpression(LogicalNot(LessEq(Var("a"), Var("b"))))
void TEST_TranslateExpression::Test5_LogicalNot_LessEq() {
    auto data = makeData({}, { Variable("a", "a", "int"), Variable("b", "b", "int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* lessEq = new ExprNode(ExprNode::LESS_EQ);
    lessEq->operands.append(a);
    lessEq->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::LOGICAL_NOT);
    root->operands.append(lessEq);

    QString result = translator->translateExpression(root);
    QCOMPARE(result, QString("a is not less than or equal to b"));

    delete root;
    delete translator;
}

//Test 6: translateExpression(LogicalNot(GreaterEq(Var("x"), Number("0"))))
void TEST_TranslateExpression::Test6_LogicalNot_GreaterEq() {
    auto data = makeData({}, { Variable("x", "x", "int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* x = new ExprNode(ExprNode::VARIABLE); x->varName = "x";
    ExprNode* zero = new ExprNode(ExprNode::NUMBER, QString("0"));
    ExprNode* greaterEq = new ExprNode(ExprNode::GREATER_EQ);
    greaterEq->operands.append(x);
    greaterEq->operands.append(zero);

    ExprNode* root = new ExprNode(ExprNode::LOGICAL_NOT);
    root->operands.append(greaterEq);

    QString result = translator->translateExpression(root);
    QCOMPARE(result, QString("x is not greater than or equal to 0"));

    delete root;
    delete translator;
}

//Test 7: translateExpression(LogicalNot(Var("flag"))),
void TEST_TranslateExpression::Test7_LogicalNot_Var() {
    auto data = makeData({}, { Variable("flag", "done flag", "bool") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* flag = new ExprNode(ExprNode::VARIABLE); flag->varName = "flag";
    ExprNode* root = new ExprNode(ExprNode::LOGICAL_NOT);
    root->operands.append(flag);

    QString result = translator->translateExpression(root);
    QCOMPARE(result, QString("not done flag"));

    delete root;
    delete translator;
}

//Test 8: translateExpression(LogicalNot(LogicalAnd(Var("a"), Var("b"))))
void TEST_TranslateExpression::Test8_LogicalNot_LogicalAnd() {
    auto data = makeData({}, { Variable("a", "cond A", "bool"), Variable("b", "cond B", "bool") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* andNode = new ExprNode(ExprNode::LOGICAL_AND);
    andNode->operands.append(a);
    andNode->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::LOGICAL_NOT);
    root->operands.append(andNode);

    QString result = translator->translateExpression(root);
    QCOMPARE(result, QString("not cond A and cond B"));

    delete root;
    delete translator;
}

//Test 9: translateExpression(Plus(PreInc(Var("i")), Var("n")))
void TEST_TranslateExpression::Test9_PreInc_InsidePlus() {
    auto data = makeData({}, { Variable("i", "i", "int"), Variable("n", "n", "int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* i = new ExprNode(ExprNode::VARIABLE); i->varName = "i";
    ExprNode* preInc = new ExprNode(ExprNode::PRE_INC);
    preInc->operands.append(i);

    ExprNode* n = new ExprNode(ExprNode::VARIABLE); n->varName = "n";

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    root->operands.append(preInc);
    root->operands.append(n);

    QString result = translator->translateExpression(root);
    QCOMPARE(result, QString("Increment i\nSum of i and n"));

    delete root;
    delete translator;
}

//Test 10: translateExpression(Minus(Var("total"), PostDec(Var("k"))))
void TEST_TranslateExpression::Test10_PostDec_InsideMinus() {
    auto data = makeData({}, { Variable("total", "total", "int"), Variable("k", "k", "int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* total = new ExprNode(ExprNode::VARIABLE); total->varName = "total";

    ExprNode* k = new ExprNode(ExprNode::VARIABLE); k->varName = "k";
    ExprNode* postDec = new ExprNode(ExprNode::POST_DEC);
    postDec->operands.append(k);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    root->operands.append(total);
    root->operands.append(postDec);

    QString result = translator->translateExpression(root);
    QCOMPARE(result, QString("Difference between total and k\nDecrement k"));

    delete root;
    delete translator;
}

//Test 11: translateExpression(LogicalOr(Var("a"), Var("b"), Var("c")))
void TEST_TranslateExpression::Test11_LogicalOr_ThreeOperands() {
    auto data = makeData({}, {
                                 Variable("a", "cond A", "bool"),
                                 Variable("b", "cond B", "bool"),
                                 Variable("c", "cond C", "bool")
                             });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";

    ExprNode* root = new ExprNode(ExprNode::LOGICAL_OR);
    root->operands.append(a);
    root->operands.append(b);
    root->operands.append(c);

    QString result = translator->translateExpression(root);
    QCOMPARE(result, QString("cond A, cond B or cond C"));

    delete root;
    delete translator;
}

#include "TEST_TranslateExpression.moc"
