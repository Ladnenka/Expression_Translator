#include <QtTest>
#include <QObject>
#include "TestUtils.h"
#include "../EnglishTranslator.h"
#include "../ExprNode.h"

class TEST_Translate : public QObject {
    Q_OBJECT
public:
    TEST_Translate() {}
    ~TEST_Translate() {}

private slots:
    void Test1_NumberLiteral();
    void Test2_VariableWithDescription();
    void Test3_VariableWithoutDescription();
    void Test4_SumTwoVars();
    void Test5_SumThreeVars();
    void Test6_SubtractTwoVars();
    void Test7_MultiplyTwoVars();
    void Test8_MultiplyThreeVars();
    void Test9_DivideTwoVars();
    void Test10_DivideSumByVar();
    void Test11_MultiplyDiffByVar();
    void Test12_SumOfDiffAndVar();
    void Test13_DiffVarAndSum();
    void Test14_Modulo();
    void Test15_Power();
    void Test16_UnaryMinusVar();
    void Test17_UnaryMinusSum();
    void Test18_LogicalAndTwo();
    void Test19_LogicalAndThree();
    void Test20_LogicalOrTwo();
    void Test21_LogicalNotVar();
    void Test22_NotLess();
    void Test23_NotGreater();
    void Test24_NotEqualOp();
    void Test25_NotNotEqual();
    void Test26_LessThan();
    void Test27_GreaterEq();
    void Test28_Assign();
    void Test29a_PlusAssign();
    void Test30_MinusAssign();
    void Test31_MultAssign();
    void Test32_DivAssign();
    void Test33_ModAssign();
    void Test34_FuncTwoVarArgs();
    void Test35_FuncSumArg();
    void Test36_FuncInDivide();
    void Test37_AddressOf();
    void Test38_DereferenceSimple();
    void Test39_DereferenceAddressOffset();
    void Test40_Index();
    void Test41_DereferenceArrayOffset();
    void Test42_DereferencePointerTo();
    void Test43_SumWithUnaryMinus();
    void Test44_SumWithNegatedFunc();
};

//Test 1: Number("42") → "42"
void TEST_Translate::Test1_NumberLiteral() {
    auto data = makeData({}, {});
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::NUMBER, QString("42"));

    QCOMPARE(translator->translate(root), QString("42"));

    delete root;
    delete translator;
}

//Test 2: Var("x") с описанием "variable x" → "variable x"
void TEST_Translate::Test2_VariableWithDescription() {
    auto data = makeData({}, { Variable("x", "variable x", "int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::VARIABLE); root->varName = "x";

    QCOMPARE(translator->translate(root), QString("variable x"));

    delete root;
    delete translator;
}

//Test 3: Var("z") без описания → "z"
void TEST_Translate::Test3_VariableWithoutDescription() {
    auto data = makeData({}, {});
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::VARIABLE); root->varName = "z";

    QCOMPARE(translator->translate(root), QString("z"));

    delete root;
    delete translator;
}

//Test 4: Plus(Var("a"), Var("b")), a→"length", b→"width" → "Sum of length and width"
void TEST_Translate::Test4_SumTwoVars() {
    auto data = makeData({}, { Variable("a", "length", "int"), Variable("b", "width", "int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    root->operands.append(a);
    root->operands.append(b);

    QCOMPARE(translator->translate(root), QString("Sum of length and width"));

    delete root;
    delete translator;
}

//Test 5: Plus(Var("a"), Var("b"), Var("c")) → "Sum of a, b and c"
void TEST_Translate::Test5_SumThreeVars() {
    auto data = makeData({}, { Variable("a","a","int"), Variable("b","b","int"), Variable("c","c","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    root->operands.append(a);
    root->operands.append(b);
    root->operands.append(c);

    QCOMPARE(translator->translate(root), QString("Sum of a, b and c"));

    delete root;
    delete translator;
}

//Test 6: Minus(Var("a"), Var("b")) → "Difference between a and b"
void TEST_Translate::Test6_SubtractTwoVars() {
    auto data = makeData({}, { Variable("a","a","int"), Variable("b","b","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    root->operands.append(a);
    root->operands.append(b);

    QCOMPARE(translator->translate(root), QString("Difference between a and b"));

    delete root;
    delete translator;
}

//Test 7: Multiply(Var("a"), Var("b")) → "Product of a and b"
void TEST_Translate::Test7_MultiplyTwoVars() {
    auto data = makeData({}, { Variable("a","a","int"), Variable("b","b","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    root->operands.append(a);
    root->operands.append(b);

    QCOMPARE(translator->translate(root), QString("Product of a and b"));

    delete root;
    delete translator;
}

//Test 8: Multiply(Var("a"), Var("b"), Var("c")) → "Product of a, b and c"
void TEST_Translate::Test8_MultiplyThreeVars() {
    auto data = makeData({}, { Variable("a","a","int"), Variable("b","b","int"), Variable("c","c","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    root->operands.append(a);
    root->operands.append(b);
    root->operands.append(c);

    QCOMPARE(translator->translate(root), QString("Product of a, b and c"));

    delete root;
    delete translator;
}

//Test 9: Divide(Var("x"), Var("y")), x→"numerator", y→"denominator"
//       → "Division of numerator by denominator"
void TEST_Translate::Test9_DivideTwoVars() {
    auto data = makeData({}, { Variable("x","numerator","int"), Variable("y","denominator","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    ExprNode* x = new ExprNode(ExprNode::VARIABLE); x->varName = "x";
    ExprNode* y = new ExprNode(ExprNode::VARIABLE); y->varName = "y";
    root->operands.append(x);
    root->operands.append(y);

    QCOMPARE(translator->translate(root), QString("Division of numerator by denominator"));

    delete root;
    delete translator;
}

//Test 10: Divide(Plus(Var("a"), Var("b")), Var("c"))
//        → "Division of (Sum of a and b) by c"
void TEST_Translate::Test10_DivideSumByVar() {
    auto data = makeData({}, { Variable("a","a","int"), Variable("b","b","int"), Variable("c","c","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    plus->operands.append(a);
    plus->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    root->operands.append(plus);
    root->operands.append(c);

    QCOMPARE(translator->translate(root), QString("Division of (Sum of a and b) by c"));

    delete root;
    delete translator;
}

//Test 11: Multiply(Minus(Var("a"), Var("b")), Var("c"))
//        → "Product of (Difference between a and b) and c"
void TEST_Translate::Test11_MultiplyDiffByVar() {
    auto data = makeData({}, { Variable("a","a","int"), Variable("b","b","int"), Variable("c","c","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* minus = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    minus->operands.append(a);
    minus->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::MULTIPLY);
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    root->operands.append(minus);
    root->operands.append(c);

    QCOMPARE(translator->translate(root), QString("Product of (Difference between a and b) and c"));

    delete root;
    delete translator;
}

//Test 12: Plus(Minus(Var("x"), Var("y")), Var("z"))
//        → "Sum of (Difference between x and y) and z"
// simplify не вызывается, Minus не раскрывается
void TEST_Translate::Test12_SumOfDiffAndVar() {
    auto data = makeData({}, { Variable("x","x","int"), Variable("y","y","int"), Variable("z","z","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* minus = new ExprNode(ExprNode::MINUS);
    ExprNode* x = new ExprNode(ExprNode::VARIABLE); x->varName = "x";
    ExprNode* y = new ExprNode(ExprNode::VARIABLE); y->varName = "y";
    minus->operands.append(x);
    minus->operands.append(y);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* z = new ExprNode(ExprNode::VARIABLE); z->varName = "z";
    root->operands.append(minus);
    root->operands.append(z);

    QCOMPARE(translator->translate(root), QString("Sum of (Difference between x and y) and z"));

    delete root;
    delete translator;
}

//Test 13: Minus(Var("a"), Plus(Var("b"), Var("c")))
//        → "Difference between a and (Sum of b and c)"
void TEST_Translate::Test13_DiffVarAndSum() {
    auto data = makeData({}, { Variable("a","a","int"), Variable("b","b","int"), Variable("c","c","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    plus->operands.append(b);
    plus->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    root->operands.append(a);
    root->operands.append(plus);

    QCOMPARE(translator->translate(root), QString("Difference between a and (Sum of b and c)"));

    delete root;
    delete translator;
}

//Test 14: Modulo(Var("a"), Var("b")) → "Remainder of a divided by b"
void TEST_Translate::Test14_Modulo() {
    auto data = makeData({}, { Variable("a","a","int"), Variable("b","b","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::MODULO);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    root->operands.append(a);
    root->operands.append(b);

    QCOMPARE(translator->translate(root), QString("Remainder of a divided by b"));

    delete root;
    delete translator;
}

//Test 15: Power(Var("b"), Number("3")) → "Exponentiation of b to 3"
void TEST_Translate::Test15_Power() {
    auto data = makeData({}, { Variable("b","b","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::POWER);
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* three = new ExprNode(ExprNode::NUMBER, QString("3"));
    root->operands.append(b);
    root->operands.append(three);

    QCOMPARE(translator->translate(root), QString("Exponentiation of b to 3"));

    delete root;
    delete translator;
}

//Test 16: UnaryMinus(Var("a")) → "negative of a"
void TEST_Translate::Test16_UnaryMinusVar() {
    auto data = makeData({}, { Variable("a","a","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::UNARY_MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    root->operands.append(a);

    QCOMPARE(translator->translate(root), QString("negative of a"));

    delete root;
    delete translator;
}

//Test 17: UnaryMinus(Plus(Var("a"), Var("b"))) → "negative of (Sum of a and b)"
void TEST_Translate::Test17_UnaryMinusSum() {
    auto data = makeData({}, { Variable("a","a","int"), Variable("b","b","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    plus->operands.append(a);
    plus->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::UNARY_MINUS);
    root->operands.append(plus);

    QCOMPARE(translator->translate(root), QString("negative of (Sum of a and b)"));

    delete root;
    delete translator;
}

//Test 18: LogicalAnd(Var("p"), Var("q")), p→"flag p", q→"flag q"
//        → "flag p and flag q"
void TEST_Translate::Test18_LogicalAndTwo() {
    auto data = makeData({}, { Variable("p","flag p","bool"), Variable("q","flag q","bool") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::LOGICAL_AND);
    ExprNode* p = new ExprNode(ExprNode::VARIABLE); p->varName = "p";
    ExprNode* q = new ExprNode(ExprNode::VARIABLE); q->varName = "q";
    root->operands.append(p);
    root->operands.append(q);

    QCOMPARE(translator->translate(root), QString("flag p and flag q"));

    delete root;
    delete translator;
}

//Test 19: LogicalAnd(Var("a"), Var("b"), Var("c"))
//        → "cond A, cond B and cond C"
void TEST_Translate::Test19_LogicalAndThree() {
    auto data = makeData({}, { Variable("a","cond A","bool"), Variable("b","cond B","bool"), Variable("c","cond C","bool") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::LOGICAL_AND);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    root->operands.append(a);
    root->operands.append(b);
    root->operands.append(c);

    QCOMPARE(translator->translate(root), QString("cond A, cond B and cond C"));

    delete root;
    delete translator;
}

//Test 20: LogicalOr(Var("p"), Var("q")), p→"cond A", q→"cond B"
//        → "cond A or cond B"
void TEST_Translate::Test20_LogicalOrTwo() {
    auto data = makeData({}, { Variable("p","cond A","bool"), Variable("q","cond B","bool") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::LOGICAL_OR);
    ExprNode* p = new ExprNode(ExprNode::VARIABLE); p->varName = "p";
    ExprNode* q = new ExprNode(ExprNode::VARIABLE); q->varName = "q";
    root->operands.append(p);
    root->operands.append(q);

    QCOMPARE(translator->translate(root), QString("cond A or cond B"));

    delete root;
    delete translator;
}

//Test 21: LogicalNot(Var("flag")), flag→"ready flag" → "not ready flag"
void TEST_Translate::Test21_LogicalNotVar() {
    auto data = makeData({}, { Variable("flag","ready flag","bool") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::LOGICAL_NOT);
    ExprNode* flag = new ExprNode(ExprNode::VARIABLE); flag->varName = "flag";
    root->operands.append(flag);

    QCOMPARE(translator->translate(root), QString("not ready flag"));

    delete root;
    delete translator;
}

//Test 22: LogicalNot(Less(Var("a"), Var("b"))) → "a is not less than b"
void TEST_Translate::Test22_NotLess() {
    auto data = makeData({}, { Variable("a","a","int"), Variable("b","b","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* less = new ExprNode(ExprNode::LESS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    less->operands.append(a);
    less->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::LOGICAL_NOT);
    root->operands.append(less);

    QCOMPARE(translator->translate(root), QString("a is not less than b"));

    delete root;
    delete translator;
}

//Test 23: LogicalNot(Greater(Var("x"), Var("y"))) → "x is not greater than y"
void TEST_Translate::Test23_NotGreater() {
    auto data = makeData({}, { Variable("x","x","int"), Variable("y","y","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* greater = new ExprNode(ExprNode::GREATER);
    ExprNode* x = new ExprNode(ExprNode::VARIABLE); x->varName = "x";
    ExprNode* y = new ExprNode(ExprNode::VARIABLE); y->varName = "y";
    greater->operands.append(x);
    greater->operands.append(y);

    ExprNode* root = new ExprNode(ExprNode::LOGICAL_NOT);
    root->operands.append(greater);

    QCOMPARE(translator->translate(root), QString("x is not greater than y"));

    delete root;
    delete translator;
}

//Test 24: LogicalNot(Equal(Var("a"), Var("b"))) → "a does not equal b"
void TEST_Translate::Test24_NotEqualOp() {
    auto data = makeData({}, { Variable("a","a","int"), Variable("b","b","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* eq = new ExprNode(ExprNode::EQUAL);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    eq->operands.append(a);
    eq->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::LOGICAL_NOT);
    root->operands.append(eq);

    QCOMPARE(translator->translate(root), QString("a does not equal b"));

    delete root;
    delete translator;
}

//Test 25: LogicalNot(NotEqual(Var("a"), Var("b"))) → "a equals b"
void TEST_Translate::Test25_NotNotEqual() {
    auto data = makeData({}, { Variable("a","a","int"), Variable("b","b","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* neq = new ExprNode(ExprNode::NOT_EQUAL);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    neq->operands.append(a);
    neq->operands.append(b);

    ExprNode* root = new ExprNode(ExprNode::LOGICAL_NOT);
    root->operands.append(neq);

    QCOMPARE(translator->translate(root), QString("a equals b"));

    delete root;
    delete translator;
}

//Test 26: Less(Var("a"), Number("0")), a→"temperature" → "temperature is less than 0"
void TEST_Translate::Test26_LessThan() {
    auto data = makeData({}, { Variable("a","temperature","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::LESS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* zero = new ExprNode(ExprNode::NUMBER, QString("0"));
    root->operands.append(a);
    root->operands.append(zero);

    QCOMPARE(translator->translate(root), QString("temperature is less than 0"));

    delete root;
    delete translator;
}

//Test 27: GreaterEq(Var("n"), Number("100")) → "n is greater than or equal to 100"
void TEST_Translate::Test27_GreaterEq() {
    auto data = makeData({}, { Variable("n","n","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::GREATER_EQ);
    ExprNode* n = new ExprNode(ExprNode::VARIABLE); n->varName = "n";
    ExprNode* hundred = new ExprNode(ExprNode::NUMBER, QString("100"));
    root->operands.append(n);
    root->operands.append(hundred);

    QCOMPARE(translator->translate(root), QString("n is greater than or equal to 100"));

    delete root;
    delete translator;
}

//Test 28: Assign(Var("x"), Number("5")) → "assignment of 5 to x"
void TEST_Translate::Test28_Assign() {
    auto data = makeData({}, { Variable("x","x","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::ASSIGN);
    ExprNode* x = new ExprNode(ExprNode::VARIABLE); x->varName = "x";
    ExprNode* five = new ExprNode(ExprNode::NUMBER, QString("5"));
    root->operands.append(x);
    root->operands.append(five);

    QCOMPARE(translator->translate(root), QString("assignment of 5 to x"));

    delete root;
    delete translator;
}

//Test 29a: PlusAssign(Var("total"), Var("delta"))
//         → "assignment of sum of total and delta to total"
void TEST_Translate::Test29a_PlusAssign() {
    auto data = makeData({}, { Variable("total","total","int"), Variable("delta","delta","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::PLUS_ASSIGN);
    ExprNode* total = new ExprNode(ExprNode::VARIABLE); total->varName = "total";
    ExprNode* delta = new ExprNode(ExprNode::VARIABLE); delta->varName = "delta";
    root->operands.append(total);
    root->operands.append(delta);

    QCOMPARE(translator->translate(root), QString("assignment of sum of total and delta to total"));

    delete root;
    delete translator;
}

//Test 30: MinusAssign(Var("balance"), Var("cost"))
//        → "assignment of difference between balance and cost to balance"
void TEST_Translate::Test30_MinusAssign() {
    auto data = makeData({}, { Variable("balance","balance","int"), Variable("cost","cost","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::MINUS_ASSIGN);
    ExprNode* balance = new ExprNode(ExprNode::VARIABLE); balance->varName = "balance";
    ExprNode* cost = new ExprNode(ExprNode::VARIABLE); cost->varName = "cost";
    root->operands.append(balance);
    root->operands.append(cost);

    QCOMPARE(translator->translate(root), QString("assignment of difference between balance and cost to balance"));

    delete root;
    delete translator;
}

//Test 31: MultAssign(Var("val"), Number("2"))
//        → "assignment of product of val and 2 to val"
void TEST_Translate::Test31_MultAssign() {
    auto data = makeData({}, { Variable("val","val","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::MULT_ASSIGN);
    ExprNode* val = new ExprNode(ExprNode::VARIABLE); val->varName = "val";
    ExprNode* two = new ExprNode(ExprNode::NUMBER, QString("2"));
    root->operands.append(val);
    root->operands.append(two);

    QCOMPARE(translator->translate(root), QString("assignment of product of val and 2 to val"));

    delete root;
    delete translator;
}

//Test 32: DivAssign(Var("avg"), Var("n")), avg→"average", n→"count"
//        → "assignment of division of average by count to average"
void TEST_Translate::Test32_DivAssign() {
    auto data = makeData({}, { Variable("avg","average","int"), Variable("n","count","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::DIV_ASSIGN);
    ExprNode* avg = new ExprNode(ExprNode::VARIABLE); avg->varName = "avg";
    ExprNode* n = new ExprNode(ExprNode::VARIABLE); n->varName = "n";
    root->operands.append(avg);
    root->operands.append(n);

    QCOMPARE(translator->translate(root), QString("assignment of division of average by count to average"));

    delete root;
    delete translator;
}

//Test 33: ModAssign(Var("r"), Number("10")), r→"remainder"
//        → "assignment of remainder of division of remainder by 10 to remainder"
void TEST_Translate::Test33_ModAssign() {
    auto data = makeData({}, { Variable("r","remainder","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::MOD_ASSIGN);
    ExprNode* r = new ExprNode(ExprNode::VARIABLE); r->varName = "r";
    ExprNode* ten = new ExprNode(ExprNode::NUMBER, QString("10"));
    root->operands.append(r);
    root->operands.append(ten);

    QCOMPARE(translator->translate(root), QString("assignment of remainder of division of remainder by 10 to remainder"));

    delete root;
    delete translator;
}

//Test 34: Function("max", [Var("a"), Var("b")]), a→"score A", b→"score B"
//        → "maximum of score A and score B"
void TEST_Translate::Test34_FuncTwoVarArgs() {
    auto data = makeData(
        { Function("max", {"a","b"}, "maximum of {a} and {b}") },
        { Variable("a","score A","int"), Variable("b","score B","int") }
        );
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    ExprNode* root = new ExprNode(ExprNode::FUNCTION, "max", QVector<ExprNode*>{a, b});

    QCOMPARE(translator->translate(root), QString("maximum of score A and score B"));

    delete root;
    delete translator;
}

//Test 35: Function("abs", [Plus(Var("x"), Var("y"))]), x→"x", y→"y"
//        → "absolute value of (Sum of x and y)"
void TEST_Translate::Test35_FuncSumArg() {
    auto data = makeData(
        { Function("abs", {"x"}, "absolute value of {x}") },
        { Variable("x","x","int"), Variable("y","y","int") }
        );
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* x = new ExprNode(ExprNode::VARIABLE); x->varName = "x";
    ExprNode* y = new ExprNode(ExprNode::VARIABLE); y->varName = "y";
    plus->operands.append(x);
    plus->operands.append(y);

    ExprNode* root = new ExprNode(ExprNode::FUNCTION, "abs", QVector<ExprNode*>{plus});

    QCOMPARE(translator->translate(root), QString("absolute value of (Sum of x and y)"));

    delete root;
    delete translator;
}

//Test 36: Divide(Function("f", [Var("x")]), Var("n"))
//        → "Division of (f of x) by n"
void TEST_Translate::Test36_FuncInDivide() {
    auto data = makeData(
        { Function("f", {"x"}, "f of {x}") },
        { Variable("x","x","int"), Variable("n","n","int") }
        );
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* x = new ExprNode(ExprNode::VARIABLE); x->varName = "x";
    ExprNode* func = new ExprNode(ExprNode::FUNCTION, "f", QVector<ExprNode*>{x});

    ExprNode* root = new ExprNode(ExprNode::DIVIDE);
    ExprNode* n = new ExprNode(ExprNode::VARIABLE); n->varName = "n";
    root->operands.append(func);
    root->operands.append(n);

    QCOMPARE(translator->translate(root), QString("Division of (f of x) by n"));

    delete root;
    delete translator;
}

//Test 37: AddressOf(Var("p")), p→"pointer p" → "Address of pointer p"
void TEST_Translate::Test37_AddressOf() {
    auto data = makeData({}, { Variable("p","pointer p","int*") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::ADDRESS_OF);
    ExprNode* p = new ExprNode(ExprNode::VARIABLE); p->varName = "p";
    root->operands.append(p);

    QCOMPARE(translator->translate(root), QString("Address of pointer p"));

    delete root;
    delete translator;
}

//Test 38: Dereference(Var("ptr")), ptr→"ptr", тип "int*" → "Value pointed to by ptr"
void TEST_Translate::Test38_DereferenceSimple() {
    auto data = makeData({}, { Variable("ptr","ptr","int*") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::DEREFERENCE);
    ExprNode* ptr = new ExprNode(ExprNode::VARIABLE); ptr->varName = "ptr";
    root->operands.append(ptr);

    QCOMPARE(translator->translate(root), QString("Value pointed to by ptr"));

    delete root;
    delete translator;
}

//Test 39: Dereference(Plus(AddressOf(Var("buf")), Number("3")))
//        → "Element at offset 3 from address of buf"
void TEST_Translate::Test39_DereferenceAddressOffset() {
    auto data = makeData({}, { Variable("buf","buf","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* buf = new ExprNode(ExprNode::VARIABLE); buf->varName = "buf";
    ExprNode* addrOf = new ExprNode(ExprNode::ADDRESS_OF);
    addrOf->operands.append(buf);

    ExprNode* three = new ExprNode(ExprNode::NUMBER, QString("3"));

    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    plus->operands.append(addrOf);
    plus->operands.append(three);

    ExprNode* root = new ExprNode(ExprNode::DEREFERENCE);
    root->operands.append(plus);

    QCOMPARE(translator->translate(root), QString("Element at offset 3 from address of buf"));

    delete root;
    delete translator;
}

//Test 40: Index(Var("arr"), Var("i")), arr→"array", i→"index i"
//        → "Element at index index i in array array"
void TEST_Translate::Test40_Index() {
    auto data = makeData({}, { Variable("arr","array","int[]"), Variable("i","index i","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::INDEX);
    ExprNode* arr = new ExprNode(ExprNode::VARIABLE); arr->varName = "arr";
    ExprNode* i = new ExprNode(ExprNode::VARIABLE); i->varName = "i";
    root->operands.append(arr);
    root->operands.append(i);

    QCOMPARE(translator->translate(root), QString("Element at index index i in array array"));

    delete root;
    delete translator;
}

//Test 41: Dereference(Plus(Var("arr"), Var("i"))), arr тип "int[]"
//        → "Element of array arr with offset index"
void TEST_Translate::Test41_DereferenceArrayOffset() {
    auto data = makeData({}, { Variable("arr","arr","int[]"), Variable("i","index","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* arr = new ExprNode(ExprNode::VARIABLE); arr->varName = "arr";
    ExprNode* i = new ExprNode(ExprNode::VARIABLE); i->varName = "i";

    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    plus->operands.append(arr);
    plus->operands.append(i);

    ExprNode* root = new ExprNode(ExprNode::DEREFERENCE);
    root->operands.append(plus);

    QCOMPARE(translator->translate(root), QString("Element of array arr with offset index"));

    delete root;
    delete translator;
}

//Test 42: Dereference(Var("ptr")), ptr→"pointer to target" → "target"
void TEST_Translate::Test42_DereferencePointerTo() {
    auto data = makeData({}, { Variable("ptr","pointer to target","int*") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* root = new ExprNode(ExprNode::DEREFERENCE);
    ExprNode* ptr = new ExprNode(ExprNode::VARIABLE); ptr->varName = "ptr";
    root->operands.append(ptr);

    QCOMPARE(translator->translate(root), QString("target"));

    delete root;
    delete translator;
}

//Test 43: Plus(Var("a"), Var("b"), UnaryMinus(Var("c")))
//        → "Sum of a, b and negative of c"
void TEST_Translate::Test43_SumWithUnaryMinus() {
    auto data = makeData({}, { Variable("a","a","int"), Variable("b","b","int"), Variable("c","c","int") });
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* c = new ExprNode(ExprNode::VARIABLE); c->varName = "c";
    ExprNode* um = new ExprNode(ExprNode::UNARY_MINUS);
    um->operands.append(c);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    root->operands.append(a);
    root->operands.append(b);
    root->operands.append(um);

    QCOMPARE(translator->translate(root), QString("Sum of a, b and negative of c"));

    delete root;
    delete translator;
}

//Test 44: Plus(Var("a"), Var("b"), UnaryMinus(Function("func", [Var("x")])))
//        → "Sum of a, b and negative of (func of x)"
void TEST_Translate::Test44_SumWithNegatedFunc() {
    auto data = makeData(
        { Function("func", {"x"}, "func of {x}") },
        { Variable("a","a","int"), Variable("b","b","int"), Variable("x","x","int") }
        );
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* x = new ExprNode(ExprNode::VARIABLE); x->varName = "x";
    ExprNode* func = new ExprNode(ExprNode::FUNCTION, "func", QVector<ExprNode*>{x});

    ExprNode* um = new ExprNode(ExprNode::UNARY_MINUS);
    um->operands.append(func);

    ExprNode* root = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    root->operands.append(a);
    root->operands.append(b);
    root->operands.append(um);

    QCOMPARE(translator->translate(root), QString("Sum of a, b and negative of (func of x)"));

    delete root;
    delete translator;
}

#include "TEST_Translate.moc"
