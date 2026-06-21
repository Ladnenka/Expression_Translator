#include "../TreeBuilder.h"

ExprNode* TreeBuilder::buildTree(const QStringList& tokens, const QString& originalExpr,
                                 const AbstractTranslator::TranslateContext::LoadedData& data,
                                 QList<Error>& errors) {
    //Очистить стек
    stack.clear();
    modifiedVariables.clear();

    //Задать счётчик позиции в строке и счётчик количества операторов в выражении
    int operationCount = 0;
    int searchFromIndex = 0;

    //Для каждого токена в выражении
    for (const QString& t : tokens) {
        while (searchFromIndex < originalExpr.size() && originalExpr[searchFromIndex] == ' ')
            searchFromIndex++;
        //Увеличить счётчик позиции в строке
        int position = searchFromIndex + 1;
        searchFromIndex += t.length();

        //Проверить тип токена
        //Если токен является числом
        //Создать узел числа, положить его в стек и завершить итерацию цикла
        if (isNumberToken(t)) { stack.push(new ExprNode(ExprNode::NUMBER, t)); continue; }

        //Если счётчик количества операций, увеличенный на один, превысил количество в 100 операций
        if (++operationCount > 100) {
            //Сохранить соответствующую ошибку в массив ошибок
            errors.append(Error(Error::TooManyOperations, "", "", "", -1, -1));
            return nullptr;
        }

        //Если токен является операцией
        //Создать соответствующий узел операции
        if (t == "+")        { if (!buildNAryOp(ExprNode::PLUS, position, errors))           return nullptr; }
        else if (t == "*")   { if (!buildNAryOp(ExprNode::MULTIPLY, position, errors))       return nullptr; }
        else if (t == "_*")  { if (!buildUnaryOp(ExprNode::DEREFERENCE, position, errors))   return nullptr; }
        else if (t == "&&")  { if (!buildNAryOp(ExprNode::LOGICAL_AND, position, errors))    return nullptr; }
        else if (t == "||")  { if (!buildNAryOp(ExprNode::LOGICAL_OR, position, errors))     return nullptr; }
        else if (t == "-")   { if (!buildBinaryOp(ExprNode::MINUS, position, errors))        return nullptr; }
        else if (t == "/")   { if (!buildBinaryOp(ExprNode::DIVIDE, position, errors))       return nullptr; }
        else if (t == "%")   { if (!buildBinaryOp(ExprNode::MODULO, position, errors))       return nullptr; }
        else if (t == "**")  { if (!buildBinaryOp(ExprNode::POWER, position, errors))        return nullptr; }
        else if (t == "=")   { if (!buildBinaryOp(ExprNode::ASSIGN, position, errors))       return nullptr; }
        else if (t == "+=")  { if (!buildBinaryOp(ExprNode::PLUS_ASSIGN, position, errors))  return nullptr; }
        else if (t == "-=")  { if (!buildBinaryOp(ExprNode::MINUS_ASSIGN, position, errors)) return nullptr; }
        else if (t == "*=")  { if (!buildBinaryOp(ExprNode::MULT_ASSIGN, position, errors))  return nullptr; }
        else if (t == "/=")  { if (!buildBinaryOp(ExprNode::DIV_ASSIGN, position, errors))   return nullptr; }
        else if (t == "%=")  { if (!buildBinaryOp(ExprNode::MOD_ASSIGN, position, errors))   return nullptr; }
        else if (t == ">")   { if (!buildBinaryOp(ExprNode::GREATER, position, errors))      return nullptr; }
        else if (t == "<")   { if (!buildBinaryOp(ExprNode::LESS, position, errors))         return nullptr; }
        else if (t == ">=")  { if (!buildBinaryOp(ExprNode::GREATER_EQ, position, errors))   return nullptr; }
        else if (t == "<=")  { if (!buildBinaryOp(ExprNode::LESS_EQ, position, errors))      return nullptr; }
        else if (t == "==")  { if (!buildBinaryOp(ExprNode::EQUAL, position, errors))        return nullptr; }
        else if (t == "!=")  { if (!buildBinaryOp(ExprNode::NOT_EQUAL, position, errors))    return nullptr; }
        else if (t == "!")   { if (!buildUnaryOp(ExprNode::LOGICAL_NOT, position, errors))   return nullptr; }
        else if (t == "&")   { if (!buildUnaryOp(ExprNode::ADDRESS_OF, position, errors))    return nullptr; }
        else if (t == "[]")  { if (!buildBinaryOp(ExprNode::INDEX, position, errors))        return nullptr; }
        else if (t == "_++") { if (!stack.isEmpty() && !checkSideEffect(stack.top(), position, errors)) return nullptr; if (!buildUnaryOp(ExprNode::PRE_INC,  position, errors)) return nullptr; }
        else if (t == "++_") { if (!stack.isEmpty() && !checkSideEffect(stack.top(), position, errors)) return nullptr; if (!buildUnaryOp(ExprNode::POST_INC, position, errors)) return nullptr; }
        else if (t == "_--") { if (!stack.isEmpty() && !checkSideEffect(stack.top(), position, errors)) return nullptr; if (!buildUnaryOp(ExprNode::PRE_DEC,  position, errors)) return nullptr; }
        else if (t == "--_") { if (!stack.isEmpty() && !checkSideEffect(stack.top(), position, errors)) return nullptr; if (!buildUnaryOp(ExprNode::POST_DEC, position, errors)) return nullptr; }
        else if (t == "_-")  { if (!buildUnaryOp(ExprNode::UNARY_MINUS, position, errors))   return nullptr; }
        else if (t == "#CALL") { if (!buildFunctionCall(position, data, errors))             return nullptr; }
        //Иначе
        else {
            operationCount--;
            //Проверить валидность токена
            for (const QChar& c : t) {
                //Если токен не валидный
                if (!c.isLetterOrNumber() && c != '_') {
                    //Сохранить соответствующую ошибку в массив ошибок
                    errors.append(Error(Error::UnsupportedOperation, t, "", "", -1, position));
                    return nullptr;
                }
            }
            //Создать узел переменной и положить его в стек
            ExprNode* v = new ExprNode(ExprNode::VARIABLE); v->varName = t;
            stack.push(v);
        }
    }

    //Если в стеке ровно один элемент и ошибок нет
    if (stack.size() == 1 && errors.isEmpty())
        //Извлечь и вернуть верхний элемент стека как корень дерева
        return stack.pop();

    //Если в стеке больше одного элемента
    if (stack.size() > 1)
        //Сохранить соответствующую ошибку в массив ошибок
        errors.append(Error(Error::ExtraOperands, "",
                            QString::number(stack.size()), "", -1, -1));

    while (!stack.isEmpty()) delete stack.pop();
    return nullptr;
}

TreeBuilder::~TreeBuilder() {
    while (!stack.isEmpty()) delete stack.pop();
}

bool TreeBuilder::isNumberToken(const QString& token) {
    return true;
}

bool TreeBuilder::buildBinaryOp(ExprNode::ExprType opType, int position, QList<Error>& errors) {
    return true;
}

bool TreeBuilder::buildUnaryOp(ExprNode::ExprType opType, int position, QList<Error>& errors) {
    return true;
}

bool TreeBuilder::buildNAryOp(ExprNode::ExprType opType, int position, QList<Error>& errors) {
    return true;
}

bool TreeBuilder::buildFunctionCall(int position,
                                    const AbstractTranslator::TranslateContext::LoadedData& data,
                                    QList<Error>& errors) {
    return true;
}

bool TreeBuilder::checkSideEffect(ExprNode* operand, int position, QList<Error>& errors) {
    return true;
}
