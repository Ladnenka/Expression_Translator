#include "../TreeBuilder.h"

TreeBuilder::TreeBuilder() {
    tokenTable["+"]   = { ExprNode::PLUS,         NARY       };
    tokenTable["*"]   = { ExprNode::MULTIPLY,      NARY       };
    tokenTable["_*"]  = { ExprNode::DEREFERENCE,   UNARY      };
    tokenTable["&&"]  = { ExprNode::LOGICAL_AND,   NARY       };
    tokenTable["||"]  = { ExprNode::LOGICAL_OR,    NARY       };
    tokenTable["-"]   = { ExprNode::MINUS,         BINARY     };
    tokenTable["/"]   = { ExprNode::DIVIDE,        BINARY     };
    tokenTable["%"]   = { ExprNode::MODULO,        BINARY     };
    tokenTable["**"]  = { ExprNode::POWER,         BINARY     };
    tokenTable["="]   = { ExprNode::ASSIGN,        BINARY     };
    tokenTable["+="]  = { ExprNode::PLUS_ASSIGN,   BINARY     };
    tokenTable["-="]  = { ExprNode::MINUS_ASSIGN,  BINARY     };
    tokenTable["*="]  = { ExprNode::MULT_ASSIGN,   BINARY     };
    tokenTable["/="]  = { ExprNode::DIV_ASSIGN,    BINARY     };
    tokenTable["%="]  = { ExprNode::MOD_ASSIGN,    BINARY     };
    tokenTable[">"]   = { ExprNode::GREATER,       BINARY     };
    tokenTable["<"]   = { ExprNode::LESS,          BINARY     };
    tokenTable[">="]  = { ExprNode::GREATER_EQ,    BINARY     };
    tokenTable["<="]  = { ExprNode::LESS_EQ,       BINARY     };
    tokenTable["=="]  = { ExprNode::EQUAL,         BINARY     };
    tokenTable["!="]  = { ExprNode::NOT_EQUAL,     BINARY     };
    tokenTable["!"]   = { ExprNode::LOGICAL_NOT,   UNARY      };
    tokenTable["&"]   = { ExprNode::ADDRESS_OF,    UNARY      };
    tokenTable["[]"]  = { ExprNode::INDEX,         BINARY     };
    tokenTable["++_"] = { ExprNode::PRE_INC,       UNARY_SIDE };
    tokenTable["_++"] = { ExprNode::POST_INC,      UNARY_SIDE };
    tokenTable["--_"] = { ExprNode::PRE_DEC,       UNARY_SIDE };
    tokenTable["_--"] = { ExprNode::POST_DEC,      UNARY_SIDE };
    tokenTable["_-"]  = { ExprNode::UNARY_MINUS,   UNARY      };
}

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

        //Если токен является числом
        //Создать узел числа, положить его в стек и завершить итерацию цикла
        if (isNumberToken(t)) { stack.push(new ExprNode(ExprNode::NUMBER, t)); continue; }

        //Если счётчик количества операций, увеличенный на один, превысил количество в 100 операций
        if (++operationCount > 100) {
            //Сохранить соответствующую ошибку в массив ошибок
            errors.append(Error(Error::TooManyOperations, "", "", "", -1, -1));
            while (!stack.isEmpty()) delete stack.pop();
            return nullptr;
        }

        //Найти токен в таблице диспетчеризации
        auto it = tokenTable.find(t);
        if (it != tokenTable.end()) {
            ExprNode::ExprType nodeType = it.value().first;
            OpKind kind                 = it.value().second;

            switch (kind) {
            case NARY:
                buildNAryOp(nodeType, position, errors);
                break;
            case BINARY:
                buildBinaryOp(nodeType, position, errors);
                break;
            case UNARY_SIDE:
                checkSideEffect(stack.isEmpty() ? nullptr : stack.top(), position, errors);
                buildUnaryOp(nodeType, position, errors);
                break;
            case UNARY:
                buildUnaryOp(nodeType, position, errors);
                break;
            }
        }
        //Если токен является вызовом функции
        else if (t == "#CALL") {
            buildFunctionCall(position, data, errors);
        }
        //Иначе: токен — переменная или неизвестная операция
        else {
            operationCount--;
            //Проверить валидность токена и создать узел переменной
            buildVariable(t, position, errors);
        }
    }

    //Если в стеке больше одного элемента
    if (stack.size() > 1)
        //Сохранить соответствующую ошибку в массив ошибок
        errors.append(Error(Error::ExtraOperands, "", QString::number(stack.size()), "", -1, -1));

    //Если в стеке ровно один элемент — извлечь и вернуть верхний элемент стека как корень дерева
    ExprNode* result = stack.size() == 1 ? stack.pop() : nullptr;
    while (!stack.isEmpty()) delete stack.pop();
    return result;
}

TreeBuilder::~TreeBuilder() {
    while (!stack.isEmpty()) delete stack.pop();
}

bool TreeBuilder::isNumberToken(const QString& token) {
    bool ok;
    token.toDouble(&ok);
    return ok;
}

void TreeBuilder::buildVariable(const QString& t, int position, QList<Error>& errors) {
    for (const QChar& c : t) {
        //Если токен не валидный
        if (!c.isLetterOrNumber() && c != '_') {
            //Сохранить соответствующую ошибку в массив ошибок
            errors.append(Error(Error::UnsupportedOperation, t, "", "", -1, position));
            stack.push(ExprNode::makeErrorNode());
            return;
        }
    }
    //Создать узел переменной и положить его в стек
    ExprNode* v = new ExprNode(ExprNode::VARIABLE);
    v->varName = t;
    stack.push(v);
}

bool TreeBuilder::buildBinaryOp(ExprNode::ExprType opType, int position, QList<Error>& errors) {
    //Проверить количество элементов в стеке
    //Если элементов стека меньше двух
    if (stack.size() < 2) {
        //Сохранить соответствующую ошибку в массив ошибок
        errors.append(Error(Error::NotEnoughOperands, "", QString::number(stack.size()), "", -1, position));
        while (stack.size() < 2)
            stack.push(ExprNode::makeErrorNode());
    }
    //Достать из стека два последних элемента из стека и сохранить их как правый и левый дочерний узел соответственно
    ExprNode* right = stack.pop();
    ExprNode* left  = stack.pop();
    //Создать узел операции переданного типа
    ExprNode* node = new ExprNode(opType);
    //Привязать к созданному узлу правый и левый дочерние узлы
    node->operands.append(left);
    node->operands.append(right);
    //Запушить созданный узел операции в стек
    stack.push(node);
    return true;
}

bool TreeBuilder::buildUnaryOp(ExprNode::ExprType opType, int position, QList<Error>& errors) {
    //Проверить количество элементов в стеке
    //Если стек пустой
    if (stack.isEmpty()) {
        //Сохранить соответствующую ошибку в массив ошибок
        errors.append(Error(Error::NotEnoughOperands, "", QString::number(stack.size()), "", -1, position));
        stack.push(ExprNode::makeErrorNode());
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
        errors.append(Error(Error::NotEnoughOperands, "", QString::number(stack.size()), "", -1, position));
        while (stack.size() < 2)
            stack.push(ExprNode::makeErrorNode());
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
    }
    //Иначе
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
        errors.append(Error(Error::NotEnoughOperands, "", QString::number(stack.size()), "", -1, position));
        stack.push(ExprNode::makeErrorNode());
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
        stack.push(ExprNode::makeErrorNode());
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
        while (stack.size() < expectedArgCount)
            stack.push(ExprNode::makeErrorNode());
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

bool TreeBuilder::checkSideEffect(const ExprNode* operand, int position, QList<Error>& errors) {
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
