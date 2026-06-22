#ifndef EXPRNODE_H
#define EXPRNODE_H
#include <QString>
#include <QVector>

/*!
 * \brief Узел дерева выражения.
 *
 * Представляет один узел в дереве разбора постфиксного выражения.
 * Каждый узел имеет тип, список дочерних узлов и дополнительные поля
 * в зависимости от типа (значение числа, имя переменной или функции).
 */
class ExprNode {
public:
    /*!
     * \brief Перечисление типов узлов дерева выражения.
     */
    enum ExprType {
        NUMBER,       ///< Числовой литерал
        VARIABLE,     ///< Переменная
        FUNCTION,     ///< Вызов функции
        PLUS,         ///< Сложение
        MINUS,        ///< Вычитание
        MULTIPLY,     ///< Умножение
        DIVIDE,       ///< Деление
        MODULO,       ///< Остаток от деления
        POWER,        ///< Возведение в степень
        UNARY_MINUS,  ///< Унарный минус
        LOGICAL_AND,  ///< Логическое И
        LOGICAL_OR,   ///< Логическое ИЛИ
        LOGICAL_NOT,  ///< Логическое НЕ
        LESS,         ///< Меньше
        GREATER,      ///< Больше
        LESS_EQ,      ///< Меньше или равно
        GREATER_EQ,   ///< Больше или равно
        EQUAL,        ///< Равно
        NOT_EQUAL,    ///< Не равно
        ASSIGN,       ///< Присваивание
        PLUS_ASSIGN,  ///< Присваивание со сложением
        MINUS_ASSIGN, ///< Присваивание с вычитанием
        MULT_ASSIGN,  ///< Присваивание с умножением
        DIV_ASSIGN,   ///< Присваивание с делением
        MOD_ASSIGN,   ///< Присваивание с остатком
        PRE_INC,      ///< Префиксный инкремент
        POST_INC,     ///< Постфиксный инкремент
        PRE_DEC,      ///< Префиксный декремент
        POST_DEC,     ///< Постфиксный декремент
        ADDRESS_OF,   ///< Взятие адреса
        DEREFERENCE,  ///< Разыменование указателя
        INDEX         ///< Индексация массива
    };

    ExprType type;              ///< Тип узла
    QVector<ExprNode*> operands; ///< Дочерние узлы (операнды)
    QString value;              ///< Значение числового литерала
    QString varName;            ///< Имя переменной
    QString funcName;           ///< Имя функции

    /*!
     * \brief Конструктор по умолчанию.
     */
    ExprNode();

    /*!
     * \brief Конструктор с типом узла.
     * \param t Тип узла
     */
    ExprNode(ExprType t);

    /*!
     * \brief Конструктор с типом узла и строковым значением.
     * \param t Тип узла
     * \param val Строковое значение (например, числовой литерал)
     */
    ExprNode(ExprType t, const QString& val);

    /*!
     * \brief Конструктор для числового узла.
     * \param num Числовое значение
     */
    ExprNode(double num);

    /*!
     * \brief Конструктор для узла переменной.
     * \param var Имя переменной
     */
    ExprNode(const QString& var);

    /*!
     * \brief Конструктор для узла вызова функции.
     * \param t Тип узла
     * \param name Имя функции
     * \param args Список узлов-аргументов
     */
    ExprNode(ExprType t, const QString& name, const QVector<ExprNode*>& args);

    /*!
     * \brief Деструктор. Рекурсивно удаляет все дочерние узлы.
     */
    ~ExprNode();
};

#endif // EXPRNODE_H