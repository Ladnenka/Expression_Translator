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

QString EnglishTranslator::translateSum(const QVector<QString>& parts)                              { return QString(); }
QString EnglishTranslator::translateSub(const QVector<QString>& parts)                              { return QString(); }
QString EnglishTranslator::translateMultiply(const QVector<QString>& parts)                         { return QString(); }
QString EnglishTranslator::translateDivide(const QVector<QString>& parts)                           { return QString(); }
QString EnglishTranslator::translateModulo(const QVector<QString>& parts)                           { return QString(); }
QString EnglishTranslator::translatePower(const QVector<QString>& parts)                            { return QString(); }
QString EnglishTranslator::translateUnaryMinus(const QVector<QString>& parts)                       { return QString(); }
QString EnglishTranslator::translateLogicalAnd(const QVector<QString>& parts)                       { return QString(); }
QString EnglishTranslator::translateLogicalOr(const QVector<QString>& parts)                        { return QString(); }
QString EnglishTranslator::translateLogicalNot(ExprNode* expr, const QVector<QString>& parts)       { return QString(); }
QString EnglishTranslator::translateLess(const QVector<QString>& parts)                             { return QString(); }
QString EnglishTranslator::translateGreater(const QVector<QString>& parts)                          { return QString(); }
QString EnglishTranslator::translateLessEq(const QVector<QString>& parts)                           { return QString(); }
QString EnglishTranslator::translateGreaterEq(const QVector<QString>& parts)                        { return QString(); }
QString EnglishTranslator::translateEqual(const QVector<QString>& parts)                            { return QString(); }
QString EnglishTranslator::translateNotEqual(const QVector<QString>& parts)                         { return QString(); }
QString EnglishTranslator::translateAssign(const QVector<QString>& parts)                           { return QString(); }
QString EnglishTranslator::translatePlusAssign(const QVector<QString>& parts)                       { return QString(); }
QString EnglishTranslator::translateMinusAssign(const QVector<QString>& parts)                      { return QString(); }
QString EnglishTranslator::translateMultAssign(const QVector<QString>& parts)                       { return QString(); }
QString EnglishTranslator::translateDivAssign(const QVector<QString>& parts)                        { return QString(); }
QString EnglishTranslator::translateModAssign(const QVector<QString>& parts)                        { return QString(); }
QString EnglishTranslator::translatePreInc(const QVector<QString>& parts)                           { return QString(); }
QString EnglishTranslator::translatePostInc(const QVector<QString>& parts)                          { return QString(); }
QString EnglishTranslator::translatePreDec(const QVector<QString>& parts)                           { return QString(); }
QString EnglishTranslator::translatePostDec(const QVector<QString>& parts)                          { return QString(); }
QString EnglishTranslator::translateAddressOf(const QVector<QString>& parts)                        { return QString(); }
QString EnglishTranslator::translateDereference(ExprNode* expr, const QVector<QString>& parts)      { return QString(); }
QString EnglishTranslator::translateIndex(const QVector<QString>& parts)                            { return QString(); }
