#include <QtTest>
#include <QObject>
#include "TestUtils.h"
#include "../EnglishTranslator.h"
#include "../ExprNode.h"

class TEST_GetFuncDescription : public QObject {
    Q_OBJECT
public:
    TEST_GetFuncDescription() {}
    ~TEST_GetFuncDescription() {}

private slots:
    void Test1_KnownNoArgs();
    void Test2_OneVarArg();
    void Test3_TwoVarArgs();
    void Test4_PlusArgWrapped();
    void Test5_MinusArgWrapped();
    void Test6_LogicalAndNoWrap();
    void Test7_NumberArg();
    void Test8_NestedFuncCall();
    void Test9_UnaryMinusNoWrap();
    void Test10_DuplicateParamInTemplate();
    void Test11_ArgIsExpression();
};

//Test 1: Вызов известной функции без аргументов
// getFuncDescription("rand", {}) → "random integer"
void TEST_GetFuncDescription::Test1_KnownNoArgs() {
    auto data = makeData({ Function("rand", {}, "random integer") }, {});
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    QString result = ctx->getFuncDescription("rand", {});
    QCOMPARE(result, QString("random integer"));

    delete translator;
}

//Test 2: Вызов функции с одним аргументом-переменной
// getFuncDescription("abs", [Var("n")]) → "absolute value of number n"
void TEST_GetFuncDescription::Test2_OneVarArg() {
    auto data = makeData(
        { Function("abs", {"x"}, "absolute value of {x}") },
        { Variable("n", "number n", "int") }
        );
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* n = new ExprNode(ExprNode::VARIABLE); n->varName = "n";
    QVector<ExprNode*> operands = { n };

    QString result = ctx->getFuncDescription("abs", operands);
    QCOMPARE(result, QString("absolute value of number n"));

    delete n;
    delete translator;
}

//Test 3: Вызов функции с двумя аргументами-переменными
// getFuncDescription("max", [Var("x"), Var("y")]) → "maximum of variable x and variable y"
void TEST_GetFuncDescription::Test3_TwoVarArgs() {
    auto data = makeData(
        { Function("max", {"a", "b"}, "maximum of {a} and {b}") },
        { Variable("x", "variable x", "int"), Variable("y", "variable y", "int") }
        );
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* x = new ExprNode(ExprNode::VARIABLE); x->varName = "x";
    ExprNode* y = new ExprNode(ExprNode::VARIABLE); y->varName = "y";
    QVector<ExprNode*> operands = { x, y };

    QString result = ctx->getFuncDescription("max", operands);
    QCOMPARE(result, QString("maximum of variable x and variable y"));

    delete x;
    delete y;
    delete translator;
}

//Test 4: Операция сложения (требует скобок)
// getFuncDescription("sqrt", [Plus(Var("a"), Var("b"))]) → "square root of (Sum of a and b)"
void TEST_GetFuncDescription::Test4_PlusArgWrapped() {
    auto data = makeData(
        { Function("sqrt", {"x"}, "square root of {x}") },
        { Variable("a", "a", "int"), Variable("b", "b", "int") }
        );
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    plus->operands.append(a);
    plus->operands.append(b);
    QVector<ExprNode*> operands = { plus };

    QString result = ctx->getFuncDescription("sqrt", operands);
    QCOMPARE(result, QString("square root of (Sum of a and b)"));

    delete plus;
    delete translator;
}

//Test 5: Операция вычитания (требует скобок)
// getFuncDescription("abs", [Minus(Var("a"), Var("b"))]) → "modulus of (Difference between a and b)"
void TEST_GetFuncDescription::Test5_MinusArgWrapped() {
    auto data = makeData(
        { Function("abs", {"x"}, "modulus of {x}") },
        { Variable("a", "a", "int"), Variable("b", "b", "int") }
        );
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* minus = new ExprNode(ExprNode::MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    minus->operands.append(a);
    minus->operands.append(b);
    QVector<ExprNode*> operands = { minus };

    QString result = ctx->getFuncDescription("abs", operands);
    QCOMPARE(result, QString("modulus of (Difference between a and b)"));

    delete minus;
    delete translator;
}

//Test 6: Логическое «И» (без скобок)
// getFuncDescription("check", [LogicalAnd(Var("a"), Var("b"))]) → "condition a and b"
void TEST_GetFuncDescription::Test6_LogicalAndNoWrap() {
    auto data = makeData(
        { Function("check", {"cond"}, "condition {cond}") },
        { Variable("a", "a", "int"), Variable("b", "b", "int") }
        );
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* landNode = new ExprNode(ExprNode::LOGICAL_AND);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    ExprNode* b = new ExprNode(ExprNode::VARIABLE); b->varName = "b";
    landNode->operands.append(a);
    landNode->operands.append(b);
    QVector<ExprNode*> operands = { landNode };

    QString result = ctx->getFuncDescription("check", operands);
    QCOMPARE(result, QString("condition a and b"));

    delete landNode;
    delete translator;
}

//Test 7: Числовой литерал
// getFuncDescription("factorial", [Number("5")]) → "factorial of 5"
void TEST_GetFuncDescription::Test7_NumberArg() {
    auto data = makeData(
        { Function("factorial", {"n"}, "factorial of {n}") },
        {}
        );
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* num = new ExprNode(ExprNode::NUMBER, QString("5"));
    QVector<ExprNode*> operands = { num };

    QString result = ctx->getFuncDescription("factorial", operands);
    QCOMPARE(result, QString("factorial of 5"));

    delete num;
    delete translator;
}

//Test 8: Вложенный вызов функции
// getFuncDescription("outer", [Function("inner", [])]) → "outer function of (inner result)"
void TEST_GetFuncDescription::Test8_NestedFuncCall() {
    auto data = makeData(
        {
            Function("outer", {"x"}, "outer function of {x}"),
            Function("inner", {}, "inner result")
        },
        {}
        );
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* inner = new ExprNode(ExprNode::FUNCTION, "inner", QVector<ExprNode*>{});
    QVector<ExprNode*> operands = { inner };

    QString result = ctx->getFuncDescription("outer", operands);
    QCOMPARE(result, QString("outer function of (inner result)"));

    delete inner;
    delete translator;
}

//Test 9: Унарный минус
// getFuncDescription("square", [UnaryMinus(Var("a"))]) → "square of negative of a"
void TEST_GetFuncDescription::Test9_UnaryMinusNoWrap() {
    auto data = makeData(
        { Function("square", {"x"}, "square of {x}") },
        { Variable("a", "a", "int") }
        );
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* um = new ExprNode(ExprNode::UNARY_MINUS);
    ExprNode* a = new ExprNode(ExprNode::VARIABLE); a->varName = "a";
    um->operands.append(a);
    QVector<ExprNode*> operands = { um };

    QString result = ctx->getFuncDescription("square", operands);
    QCOMPARE(result, QString("square of negative of a"));

    delete um;
    delete translator;
}

//Test 10: Дублирование параметра в шаблоне
// getFuncDescription("square", [Var("n")]) → "n multiplied by n"
void TEST_GetFuncDescription::Test10_DuplicateParamInTemplate() {
    auto data = makeData(
        { Function("square", {"x"}, "{x} multiplied by {x}") },
        { Variable("n", "n", "int") }
        );
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* n = new ExprNode(ExprNode::VARIABLE); n->varName = "n";
    QVector<ExprNode*> operands = { n };

    QString result = ctx->getFuncDescription("square", operands);
    QCOMPARE(result, QString("n multiplied by n"));

    delete n;
    delete translator;
}

//Test 11 : Аргумент — выражение Plus(Var("x"), Number("2"))
// getFuncDescription("mul", [Plus(Var("x"), Number("2")), Var("y")]) → "mul (Sum of x and 2) by y"
void TEST_GetFuncDescription::Test11_ArgIsExpression() {
    auto data = makeData(
        { Function("mul", {"a", "b"}, "mul {a} by {b}") },
        { Variable("x", "x", "int"), Variable("y", "y", "int") }
        );
    auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
    EnglishTranslator* translator = new EnglishTranslator(ctx);

    ExprNode* plus = new ExprNode(ExprNode::PLUS);
    ExprNode* x = new ExprNode(ExprNode::VARIABLE); x->varName = "x";
    ExprNode* two = new ExprNode(ExprNode::NUMBER, QString("2"));
    plus->operands.append(x);
    plus->operands.append(two);

    ExprNode* y = new ExprNode(ExprNode::VARIABLE); y->varName = "y";
    QVector<ExprNode*> operands = { plus, y };

    QString result = ctx->getFuncDescription("mul", operands);
    QCOMPARE(result, QString("mul (Sum of x and 2) by y"));

    delete plus;
    delete y;
    delete translator;
}

#include "TEST_GetFunctionDescription.moc"
