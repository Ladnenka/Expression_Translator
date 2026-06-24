#include "../AbstractTranslator.h"

AbstractTranslator::TranslateContext::~TranslateContext() {}

AbstractTranslator::AbstractTranslator(TranslateContext* ctx) : context(ctx) {
    context->translator = this;
}

AbstractTranslator::~AbstractTranslator() { delete context; }

AbstractTranslator::TranslateContext::LoadedData::LoadedData() {}

QString AbstractTranslator::translateExpression(ExprNode* root) {

    preLines.clear();
    postLines.clear();

    QString mainResult = translate(root);

    return (preLines + QStringList{mainResult} + postLines).join("\n");
}

QString AbstractTranslator::translate(ExprNode* expr) {
    //Проверить существует ли переданный узел
    //Если указатель на узел отсутствует
    //Вернуть пустую строку
    if (!expr) return "";

    //Создать список строк операндов
    QVector<QString> operandStrings;
    //Для каждого дочернего узла, привязанного к переданному узлу
    for (ExprNode* operand : expr->operands) {
        //Рекурсивно вызвать метод перевода для дочернего узла
        //Сохранить результат как строку операнда
        QString operandStr = translate(operand);
        //Проверить необходимость добавления скобок
        //Если для текущего операнда требуются скобки
        if (needParentheses(expr->type, operand->type)) {
            //Добавить открывающую и закрывающую скобки вокруг строки операнда
            operandStr = "(" + operandStr + ")";
        }
        //Добавить полученную строку в список строк операндов
        operandStrings.push_back(operandStr);
    }

    //Определить тип переданного узла
    //В зависимости от типа переданного узла
    //Вызвать соответствующий метод перевода
    switch (expr->type) {
    case ExprNode::NUMBER:        return translateNumber(expr);
    case ExprNode::VARIABLE:      return context->getVarDescription(expr->varName);
    case ExprNode::FUNCTION:      return context->getFuncDescription(expr->funcName, expr->operands);
    case ExprNode::PLUS:          return translateSum(operandStrings);
    case ExprNode::MINUS:         return translateSub(operandStrings);
    case ExprNode::MULTIPLY:      return translateMultiply(operandStrings);
    case ExprNode::DIVIDE:        return translateDivide(operandStrings);
    case ExprNode::MODULO:        return translateModulo(operandStrings);
    case ExprNode::POWER:         return translatePower(operandStrings);
    case ExprNode::UNARY_MINUS:   return translateUnaryMinus(operandStrings);
    case ExprNode::LOGICAL_AND:   return translateLogicalAnd(operandStrings);
    case ExprNode::LOGICAL_OR:    return translateLogicalOr(operandStrings);
    case ExprNode::LOGICAL_NOT:   return translateLogicalNot(expr, operandStrings);
    case ExprNode::LESS:          return translateLess(operandStrings);
    case ExprNode::GREATER:       return translateGreater(operandStrings);
    case ExprNode::LESS_EQ:       return translateLessEq(operandStrings);
    case ExprNode::GREATER_EQ:    return translateGreaterEq(operandStrings);
    case ExprNode::EQUAL:         return translateEqual(operandStrings);
    case ExprNode::NOT_EQUAL:     return translateNotEqual(operandStrings);
    case ExprNode::ASSIGN:        return translateAssign(operandStrings);
    case ExprNode::PLUS_ASSIGN:   return translatePlusAssign(operandStrings);
    case ExprNode::MINUS_ASSIGN:  return translateMinusAssign(operandStrings);
    case ExprNode::MULT_ASSIGN:   return translateMultAssign(operandStrings);
    case ExprNode::DIV_ASSIGN:    return translateDivAssign(operandStrings);
    case ExprNode::MOD_ASSIGN:    return translateModAssign(operandStrings);
    case ExprNode::PRE_INC:       return translatePreInc(operandStrings);
    case ExprNode::POST_INC:      return translatePostInc(operandStrings);
    case ExprNode::PRE_DEC:       return translatePreDec(operandStrings);
    case ExprNode::POST_DEC:      return translatePostDec(operandStrings);
    case ExprNode::ADDRESS_OF:    return translateAddressOf(operandStrings);
    case ExprNode::DEREFERENCE:   return translateDereference(expr, operandStrings);
    case ExprNode::INDEX:         return translateIndex(operandStrings);
    default: return "";
    }
}

bool AbstractTranslator::needParentheses(ExprNode::ExprType exprType, ExprNode::ExprType operandType) {return false;}

QString AbstractTranslator::translateNumber(const ExprNode* expr) { return expr->value; }
