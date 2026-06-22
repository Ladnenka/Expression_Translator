#include "../EnglishTranslator.h"

EnglishTranslator::EnglishTranslateContext::EnglishTranslateContext(const AbstractTranslator::TranslateContext::LoadedData& data)
    : TranslateContext(data) {}

EnglishTranslator::EnglishTranslator(EnglishTranslateContext* ctx)
    : AbstractTranslator(ctx) {}

QString EnglishTranslator::EnglishTranslateContext::getFuncDescription(const QString& funcName, const QVector<ExprNode*>& operands) {
    //Для каждой функции из списка функций в структуре загруженных данных
    for (const Function& f : loadedData.functions) {
        //Если переданное имя функции равно текущему имени функции
        if (f.name == funcName) {
            //Создать новый список строк для переведённых аргументов
            QStringList args;
            //Для каждого привязанного узла из списка переданных узлов аргументов функции
            for (ExprNode* op : operands) {
                //Выполнить перевод узла через переводчик и сохранить результат
                QString translated = translator->translate(op);

                //Если тип текущего привязанного узла равен сложению вычитанию, умножению, делению, возведению в степень или остатку от деления
                //Добавить сохранённый результат в список строк, обернув его в круглые скобки
                if (op->type == ExprNode::PLUS || op->type == ExprNode::MINUS ||
                    op->type == ExprNode::MULTIPLY || op->type == ExprNode::DIVIDE ||
                    op->type == ExprNode::POWER || op->type == ExprNode::MODULO  || op->type == ExprNode::FUNCTION)
                    args << "(" + translated + ")";
                //Иначе
                else
                    //Добавить сохранённый результат перевода в список без изменений
                    args << translated;
            }

            //Сохранить шаблон описания текущей функции в результирующую строку
            QString result = f.templateDesc;
            // Для каждой пары «параметр–аргумент» выполнить замену имени параметра в результирующей строке
            for (int i = 0; i < args.size() && i < f.paramNames.size(); i++)
                result.replace("{" + f.paramNames[i] + "}", args[i]);
            //Вернуть результирующую строку
            return result;
        }
    }

    //Создать новый список строк для аргументов функции по умолчанию
    QStringList args;
    //Для каждого привязанного узла из переданного вектора дочерних узлов
    for (ExprNode* op : operands) {
        //Выполнить перевод текущего привязанного узла через переводчик и добавить результат в список строк
        args << translator->translate(op);
    }
    //Возвратить строку, состоящую из переданного имени функции, открывающей круглой скобки, всех элементов списка строк аргументов функции и закрывающей скобки
    return funcName + "(" + args.join(", ") + ")";
}

QString EnglishTranslator::EnglishTranslateContext::getVarDescription(const QString& varName) {
    for (const Variable& v : loadedData.variables)
        if (v.name == varName) return v.description;
    return varName;
}

bool EnglishTranslator::needParentheses(ExprNode::ExprType parentType,
                                        ExprNode::ExprType operandType) {
    switch (parentType) {
    case ExprNode::DIVIDE:
        return operandType == ExprNode::PLUS     ||
               operandType == ExprNode::MINUS    ||
               operandType == ExprNode::MULTIPLY ||
               operandType == ExprNode::DIVIDE   ||
               operandType == ExprNode::FUNCTION;

    case ExprNode::MULTIPLY:
        return operandType == ExprNode::PLUS  ||
               operandType == ExprNode::MINUS ||
               operandType == ExprNode::FUNCTION ||
               operandType == ExprNode::DIVIDE;

    case ExprNode::PLUS:
        return operandType == ExprNode::MINUS    ||
               operandType == ExprNode::MULTIPLY ||
               operandType == ExprNode::DIVIDE   ||
               operandType == ExprNode::FUNCTION;

    case ExprNode::MINUS:
        return operandType == ExprNode::PLUS     ||
               operandType == ExprNode::MINUS    ||
               operandType == ExprNode::MULTIPLY ||
               operandType == ExprNode::DIVIDE   ||
               operandType == ExprNode::FUNCTION;

    case ExprNode::UNARY_MINUS:
        return operandType == ExprNode::PLUS     ||
               operandType == ExprNode::MINUS    ||
               operandType == ExprNode::MULTIPLY ||
               operandType == ExprNode::DIVIDE   ||
               operandType == ExprNode::FUNCTION;

    default:
        return false;
    }
}

QString EnglishTranslator::translateSum(const QVector<QString>& parts) {
    //Проверить количество элементов в списке строк
    //Если список содержит один элемент
    //Вернуть этот элемент
    if (parts.size() == 1) return parts[0];
    //Если размер списка равен двум
    //Вернуть строку, состоящую из строки "Sum of", первого элемента списка, строки "and" и второго элемента списка
    if (parts.size() == 2) return "Sum of " + parts[0] + " and " + parts[1];

    //Сохранить в результирующую строку "Sum of" и первый элемент списка строк
    QString result = "Sum of " + parts[0];
    //Для каждого элемента в списке строк, начиная со второго и до предпоследнего
    for (int i = 1; i < parts.size() - 1; i++)
        //Добавить к результирующей строке запятую с пробелом и текущий элемент списка
        result += ", " + parts[i];
    //Добавить к результирующей строке текст " and" и последний элемент списка строк
    result += " and " + parts.last();
    //Возвратить полученную строку
    return result;
}

QString EnglishTranslator::translateSub(const QVector<QString>& parts) {
    return "Difference between " + parts[0] + " and " + parts[1];
}

QString EnglishTranslator::translateMultiply(const QVector<QString>& parts) {
    //Проверить количество элементов в списке строк
    //Если список содержит один элемент
    //Вернуть этот элемент
    if (parts.size() == 1) return parts[0];
    //Если размер списка равен двум
    //Вернуть строку, состоящую из строки "Product of", первого элемента списка, строки "and" и второго элемента списка
    if (parts.size() == 2) return "Product of " + parts[0] + " and " + parts[1];

    //Сохранить в результирующую строку "Product of" и первый элемент списка строк
    QString result = "Product of " + parts[0];
    //Для каждого элемента в списке строк, начиная со второго и до предпоследнего
    for (int i = 1; i < parts.size() - 1; i++)
        //Добавить к результирующей строке запятую с пробелом и текущий элемент списка
        result += ", " + parts[i];
    //Добавить к результирующей строке текст " and" и последний элемент списка строк
    result += " and " + parts.last();
    //Возвратить полученную строку
    return result;
}

QString EnglishTranslator::translateDivide(const QVector<QString>& parts) {
    return "Division of " + parts[0] + " by " + parts[1];
}

QString EnglishTranslator::translateModulo(const QVector<QString>& parts) {
    return "Remainder of " + parts[0] + " divided by " + parts[1];
}

QString EnglishTranslator::translatePower(const QVector<QString>& parts) {
    return "Exponentiation of " + parts[0] + " to " + parts[1];
}

QString EnglishTranslator::translateUnaryMinus(const QVector<QString>& parts) {
    return "negative of " + parts[0];
}

QString EnglishTranslator::translateLogicalAnd(const QVector<QString>& parts) {
    if (parts.size() == 2) return parts[0] + " and " + parts[1];

    QString result = parts[0];
    for (int i = 1; i < parts.size() - 1; i++)
        result += ", " + parts[i];
    result += " and " + parts.last();
    return result;
}

QString EnglishTranslator::translateLogicalOr(const QVector<QString>& parts) {
    if (parts.size() == 2) return parts[0] + " or " + parts[1];

    QString result = parts[0];
    for (int i = 1; i < parts.size() - 1; i++)
        result += ", " + parts[i];
    result += " or " + parts.last();
    return result;
}

QString EnglishTranslator::translateLogicalNot(ExprNode* expr,
                                               const QVector<QString>& parts) {
    //Взять указатель на узел выражения, которое отрицается
    ExprNode* child = expr->operands[0];

    //Определить тип узла этого выражения
    switch (child->type) {
    //Если выражение является операцией сравнения, равенства или неравенства
    case ExprNode::LESS:
    case ExprNode::LESS_EQ:
    case ExprNode::GREATER:
    case ExprNode::GREATER_EQ:
    case ExprNode::EQUAL:
    case ExprNode::NOT_EQUAL: {
        //Перевести первый дочерний узел узла выражения и сохранить результат
        QString left  = translate(child->operands[0]);
        //Перевести второй дочерний узел узла выражения и сохранить результат
        QString right = translate(child->operands[1]);

        //В зависимости от типа операции
        //Сформировать и вернуть соответствующую строку перевода отрицания данной операции
        switch (child->type) {
        case ExprNode::LESS:       return left + " is not less than " + right;
        case ExprNode::LESS_EQ:    return left + " is not less than or equal to " + right;
        case ExprNode::GREATER:    return left + " is not greater than " + right;
        case ExprNode::GREATER_EQ: return left + " is not greater than or equal to " + right;
        case ExprNode::EQUAL:      return left + " does not equal " + right;
        case ExprNode::NOT_EQUAL:  return left + " equals " + right;
        default: break;
        }
    }
     //Для любого другого типа выражения
    default:
        //Добавить перед переводом выражения строку "not"
        return "not " + parts[0];
    }
}

QString EnglishTranslator::translateLess(const QVector<QString>& parts) {
    return parts[0] + " is less than " + parts[1];
}

QString EnglishTranslator::translateGreater(const QVector<QString>& parts) {
    return parts[0] + " is greater than " + parts[1];
}

QString EnglishTranslator::translateLessEq(const QVector<QString>& parts) {
    return parts[0] + " is less than or equal to " + parts[1];
}

QString EnglishTranslator::translateGreaterEq(const QVector<QString>& parts) {
    return parts[0] + " is greater than or equal to " + parts[1];
}

QString EnglishTranslator::translateEqual(const QVector<QString>& parts) {
    return parts[0] + " equals " + parts[1];
}

QString EnglishTranslator::translateNotEqual(const QVector<QString>& parts) {
    return parts[0] + " does not equal " + parts[1];
}

QString EnglishTranslator::translateAssign(const QVector<QString>& parts) {
    return "assignment of " + parts[1] + " to " + parts[0];
}

QString EnglishTranslator::translatePlusAssign(const QVector<QString>& parts) {
    return "assignment of sum of " + parts[0] + " and " + parts[1] + " to " + parts[0];
}

QString EnglishTranslator::translateMinusAssign(const QVector<QString>& parts) {
    return "assignment of difference between " + parts[0] + " and " + parts[1] + " to " + parts[0];
}

QString EnglishTranslator::translateMultAssign(const QVector<QString>& parts) {
    return "assignment of product of " + parts[0] + " and " + parts[1] + " to " + parts[0];
}

QString EnglishTranslator::translateDivAssign(const QVector<QString>& parts) {
    return "assignment of division of " + parts[0] + " by " + parts[1] + " to " + parts[0];
}

QString EnglishTranslator::translateModAssign(const QVector<QString>& parts) {
    return "assignment of remainder of division of " + parts[0] + " by " + parts[1] + " to " + parts[0];
}

QString EnglishTranslator::translatePreInc(const QVector<QString>& parts) {
    preLines << "Increment " + parts[0];
    return parts[0];
}

QString EnglishTranslator::translatePostInc(const QVector<QString>& parts) {
    postLines << "Increment " + parts[0];
    return parts[0];
}

QString EnglishTranslator::translatePreDec(const QVector<QString>& parts) {
    preLines << "Decrement " + parts[0];
    return parts[0];
}

QString EnglishTranslator::translatePostDec(const QVector<QString>& parts) {
    postLines << "Decrement " + parts[0];
    return parts[0];
}

QString EnglishTranslator::translateAddressOf(const QVector<QString>& parts)                        { return QString(); }
QString EnglishTranslator::translateDereference(ExprNode* expr, const QVector<QString>& parts)      { return QString(); }
QString EnglishTranslator::translateIndex(const QVector<QString>& parts)                            { return QString(); }
