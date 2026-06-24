#ifndef ABSTRACTTRANSLATOR_H
#define ABSTRACTTRANSLATOR_H
#include "Variable.h"
#include "Function.h"
#include <QHash>
#include <QString>
#include <QStringList>
#include "ExprNode.h"

/*!
 * \file AbstractTranslator.h
 * \brief Абстрактный базовый класс переводчика выражений.
 */

/*!
 * \brief Абстрактный базовый класс переводчика выражений.
 *
 * Реализует обход дерева выражения и делегирует перевод каждого типа узла
 * виртуальным методам, которые переопределяются в конкретных переводчиках.
 */
class AbstractTranslator
{
public:

    /*!
     * \brief Абстрактный контекст переводчика.
     *
     * Хранит загруженные данные о переменных и функциях,
     * а также предоставляет методы для получения их описаний.
     */
    class TranslateContext {
    public:

        /*!
         * \brief Структура для хранения загруженных данных о переменных и функциях.
         */
        struct LoadedData {
            QList<Variable> variables;          ///< Список загруженных переменных
            QList<Function> functions;          ///< Список загруженных функций
            QStringList functionNames;          ///< Список имён функций
            QHash<QString, int> functionArgCount; ///< Количество аргументов для каждой функции

            /*!
             * \brief Конструктор по умолчанию.
             */
            LoadedData();

            /*!
             * \brief Конструктор с параметрами.
             * \param[in] vars Список переменных
             * \param [in] funcs Список функций
             */
            LoadedData(const QList<Variable>& vars, const QList<Function>& funcs);
        };

        /*!
         * \brief Конструктор контекста.
         * \param [in] data Загруженные данные о переменных и функциях
         */
        TranslateContext(const LoadedData& data) : loadedData(data) {}

        /*!
         * \brief Виртуальный деструктор.
         */
        virtual ~TranslateContext();

        /*!
         * \brief Возвращает тип переменной по её имени.
         * \param [in] varName Имя переменной
         * \return Строка с типом переменной
         */
        virtual QString getVarType(const QString& varName) = 0;

        /*!
         * \brief Возвращает текстовое описание переменной по её имени.
         * \param [in] varName Имя переменной
         * \return Строка с описанием переменной
         */
        virtual QString getVarDescription(const QString& varName) = 0;

        /*!
         * \brief Возвращает текстовое описание вызова функции.
         * \param [in] funcName Имя функции
         * \param [in] operands Список узлов-аргументов
         * \return Строка с описанием вызова функции
         */
        virtual QString getFuncDescription(const QString& funcName, const QVector<ExprNode*>& operands) = 0;

        AbstractTranslator* translator = nullptr; ///< Указатель на связанный переводчик
    protected:
        LoadedData loadedData; ///< Загруженные данные о переменных и функциях
    };

protected:
    TranslateContext* context; ///< Контекст переводчика
    QStringList preLines;      ///< Строки, добавляемые перед результатом перевода
    QStringList postLines;     ///< Строки, добавляемые после результата перевода

    /*!
     * \brief Определяет, нужны ли скобки вокруг операнда.
     * \param [in] exprType Тип родительского узла
     * \param [in] operandType Тип дочернего узла
     * \return true если скобки необходимы
     */
    virtual bool needParentheses(ExprNode::ExprType exprType, ExprNode::ExprType operandType);

    /*!
     * \brief Переводит числовой узел в строку.
     * \param [in] expr Указатель на узел числа
     * \return Строковое представление числа
     */
    static QString translateNumber(const ExprNode* expr);

    virtual QString translateSum(const QVector<QString>& parts) = 0;          ///< Перевод сложения
    virtual QString translateSub(const QVector<QString>& parts) = 0;          ///< Перевод вычитания
    virtual QString translateMultiply(const QVector<QString>& parts) = 0;     ///< Перевод умножения
    virtual QString translateDivide(const QVector<QString>& parts) = 0;       ///< Перевод деления
    virtual QString translateModulo(const QVector<QString>& parts) = 0;       ///< Перевод остатка от деления
    virtual QString translatePower(const QVector<QString>& parts) = 0;        ///< Перевод возведения в степень
    virtual QString translateUnaryMinus(const QVector<QString>& parts) = 0;   ///< Перевод унарного минуса
    virtual QString translateLogicalAnd(const QVector<QString>& parts) = 0;   ///< Перевод логического И
    virtual QString translateLogicalOr(const QVector<QString>& parts) = 0;    ///< Перевод логического ИЛИ
    virtual QString translateLogicalNot(ExprNode* expr, const QVector<QString>& parts) = 0; ///< Перевод логического НЕ
    virtual QString translateLess(const QVector<QString>& parts) = 0;         ///< Перевод операции меньше
    virtual QString translateGreater(const QVector<QString>& parts) = 0;      ///< Перевод операции больше
    virtual QString translateLessEq(const QVector<QString>& parts) = 0;       ///< Перевод операции меньше или равно
    virtual QString translateGreaterEq(const QVector<QString>& parts) = 0;    ///< Перевод операции больше или равно
    virtual QString translateEqual(const QVector<QString>& parts) = 0;        ///< Перевод операции равно
    virtual QString translateNotEqual(const QVector<QString>& parts) = 0;     ///< Перевод операции не равно
    virtual QString translateAssign(const QVector<QString>& parts) = 0;       ///< Перевод присваивания
    virtual QString translatePlusAssign(const QVector<QString>& parts) = 0;   ///< Перевод присваивания со сложением
    virtual QString translateMinusAssign(const QVector<QString>& parts) = 0;  ///< Перевод присваивания с вычитанием
    virtual QString translateMultAssign(const QVector<QString>& parts) = 0;   ///< Перевод присваивания с умножением
    virtual QString translateDivAssign(const QVector<QString>& parts) = 0;    ///< Перевод присваивания с делением
    virtual QString translateModAssign(const QVector<QString>& parts) = 0;    ///< Перевод присваивания с остатком
    virtual QString translatePreInc(const QVector<QString>& parts) = 0;       ///< Перевод префиксного инкремента
    virtual QString translatePostInc(const QVector<QString>& parts) = 0;      ///< Перевод постфиксного инкремента
    virtual QString translatePreDec(const QVector<QString>& parts) = 0;       ///< Перевод префиксного декремента
    virtual QString translatePostDec(const QVector<QString>& parts) = 0;      ///< Перевод постфиксного декремента
    virtual QString translateAddressOf(const QVector<QString>& parts) = 0;    ///< Перевод взятия адреса
    virtual QString translateDereference(ExprNode* expr, const QVector<QString>& parts) = 0; ///< Перевод разыменования
    virtual QString translateIndex(const QVector<QString>& parts) = 0;        ///< Перевод индексации массива

public:
    /*!
     * \brief Конструктор переводчика.
     * \param [in] ctx Указатель на контекст переводчика
     */
    AbstractTranslator(TranslateContext* ctx);

    /*!
     * \brief Виртуальный деструктор. Удаляет контекст.
     */
    virtual ~AbstractTranslator();

    /*!
     * \brief Выполняет полный перевод выражения, представленного деревом.
     *
     * Очищает preLines и postLines, вызывает translate(root) и объединяет результат.
     *
     * \param [in] root Корень дерева выражения
     * \return Строка с переводом выражения
     */
    QString translateExpression(ExprNode* root);

    /*!
     * \brief Рекурсивно переводит узел дерева выражения.
     *
     * Обходит дерево в глубину, переводит каждый дочерний узел,
     * при необходимости добавляет скобки и вызывает соответствующий метод перевода.
     *
     * \param [in] expr Указатель на текущий узел
     * \return Строка с переводом узла
     */
    QString translate(ExprNode* expr);

    //ccpcheck recomendation
    /*!
     * \brief Запрещённый конструктор копирования
     *
     * Копирование объектов переводчика не допускается, поскольку объект
     * владеет контекстом перевода и внутренним состоянием.
     *
     * \param [in] other Объект, который предполагалось скопировать
     */
    AbstractTranslator(const AbstractTranslator&) = delete;

    /*!
     * \brief Запрещённый оператор копирующего присваивания
     *
     * Присваивание одного объекта переводчика другому не допускается,
     * чтобы избежать некорректного копирования внутреннего состояния.
     *
     * \param [in] other Объект-источник присваивания
     */
    AbstractTranslator& operator=(const AbstractTranslator&) = delete;
};

#endif // ABSTRACTTRANSLATOR_H
