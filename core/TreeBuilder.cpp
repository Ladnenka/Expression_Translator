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
    bool ok;
    token.toDouble(&ok);
    return ok;
}

bool TreeBuilder::buildBinaryOp(ExprNode::ExprType opType, int position, QList<Error>& errors) {
    //Проверить количество элементов в стеке
    //Если элементов стека меньше двух
    if (stack.size() < 2) {
        //Сохранить соответствующую ошибку в массив ошибок
        errors.append(Error(Error::NotEnoughOperands, "", "", "", -1, position));
        return false;
    }
    //Достать из стека два последних элемента из стека и сохранить их как правый и левый дочерний узел соответственно
    ExprNode* right = stack.pop();
    ExprNode* left  = stack.pop();\
    //Создать узел операции переданного типа
    ExprNode* node = new ExprNode(opType);
    //Привязать к созданному узлу правый и левый дочерние узлы
    node->operands.append(left);
    node->operands.append(right);
    //Запушить созданный узел операции в стек
    stack.push(node);
    return true;
}

bool TreeBuilder::buildUnaryOp(ExprNode::ExprType opType, int position, QList<Error>& errors) {\
    //Проверить количество элементов в стеке
    //Если стек пустой
    if (stack.isEmpty()) {
        //Сохранить соответствующую ошибку в массив ошибок
        errors.append(Error(Error::NotEnoughOperands, "", "", "", -1, position));
        return false;
    }
    //Достать из стека последний элемент и сохранить его
    ExprNode* operand = stack.pop();
    //Создать узел операции переданного типа
    ExprNode* node = new ExprNode(opType);
    //Привязать к созданному узлу сохранённый узел
    node->operands.append(operand);
    //Запушить созданный узел операции в стек
    stack.push(node);
    return true;
}

bool TreeBuilder::buildNAryOp(ExprNode::ExprType opType, int position, QList<Error>& errors) {
    //Проверить количество элементов в стеке
    //Если элементов стека меньше двух
    if (stack.size() < 2) {
        //Сохранить соответствующую ошибку в массив ошибок
        errors.append(Error(Error::NotEnoughOperands, "", "", "", -1, position));
        return false;
    }
    //Достать из стека два последних элемента из стека и сохранить их как правый и левый дочерний узел соответственно
    ExprNode* right = stack.pop();
    ExprNode* left  = stack.pop();
    //Создать узел операции переданного типа
    ExprNode* node  = new ExprNode(opType);
    //Если тип левого дочернего узла такой же как и у созданного узла
    if (left->type == opType) {
        //Привязать все узлы дочерние от левого дочернего узла к созданному узлу
        for (ExprNode* op : left->operands) node->operands.append(op);
        left->operands.clear(); delete left;
    }
    //Иначе
    //Привязать левый дочерний узел к созданному узлу
    else node->operands.append(left);
    //Если тип правого дочернего узла такой же как и у созданного узла
    //Привязать все узлы дочерние от правого дочернего узла к созданному узлу
    if (right->type == opType) {
        for (ExprNode* op : right->operands) node->operands.append(op);
        right->operands.clear(); delete right;
    } //Иначе
    //Привязать правый дочерний узел к созданному узлу
    else node->operands.append(right);
    //Запушить созданный узел операции в стек
    stack.push(node);
    return true;
}

bool TreeBuilder::buildFunctionCall(int position,
                                    const AbstractTranslator::TranslateContext::LoadedData& data,
                                    QList<Error>& errors) {
    //Проверить количество элементов в стеке
    //Если стек пустой
    if (stack.isEmpty()) {
        //Сохранить соответствующую ошибку в массив ошибок
        errors.append(Error(Error::NotEnoughOperands, "", "", "", -1, position));
        return false;
    }

    //Достать из стека последний узел и сохранить его значение как имя функции, после чего удалить узел
    ExprNode* nameNode = stack.pop();
    QString funcName = nameNode->varName;
    delete nameNode;

    //Проверить наличие функции в списке загруженных функций
    //Если функция не найдена
    if (!data.functionNames.contains(funcName)) {
        //Сохранить соответствующую ошибку в массив ошибок
        errors.append(Error(Error::UnknownFunction, funcName, "", "", -1, position));
        return false;
    }

    //Проверить, что количество элементов в стеке соответствует количеству ожидаемых параметров у функции
    int expectedArgCount = data.functionArgCount.value(funcName, 2);
    int actualArgCount = stack.size();

    //Если количество полученных(найденных на стеке) параметров меньше ожидаемого количества параметров
    if (actualArgCount < expectedArgCount) {
        //Сохранить соответствующую ошибку в массив ошибок
        QString dataTypeStr = QString::number(expectedArgCount) + "|" + QString::number(actualArgCount);
        errors.append(Error(Error::InvalidArgumentCount, funcName, dataTypeStr, "", -1, position));
        return false;
    }

    //Достать все элементы со стека и сохранить их в список параметров функции
    QVector<ExprNode*> args;
    for (int i = 0; i < expectedArgCount; i++) {
        args.prepend(stack.pop());
    }


    //Создать узел функции и привязать к нему список параметров функции
    //Запушить созданный узел операции в стек
    stack.push(new ExprNode(ExprNode::FUNCTION, funcName, args));
    return true;
}

bool TreeBuilder::checkSideEffect(ExprNode* operand, int position, QList<Error>& errors) {
    //Проверить переданный указатель на узел верхний узел в стеке
    //Если проверяемый узел не переменная и указатель на узел не пустой, то завершить выполнение метода
    if (!operand || operand->type != ExprNode::VARIABLE) return true;

    //Сохранить значение узла как имя переменной
    QString varName = operand->varName;
    //Проверить находится ли данная переменная в списке уже изменённых переменных
    //Если переменная была найдена в списке
    if (modifiedVariables.contains(varName)) {
        //Сохранить соответствующую ошибку в массив ошибок
        errors.append(Error(Error::SideEffectConflict, varName, "", "", -1, position));
        return false;
    }
    //Добавить переменную в список изменённых переменных
    modifiedVariables.insert(varName);
    return true;
}

