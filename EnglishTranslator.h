#ifndef ENGLISHTRANSLATOR_H
#define ENGLISHTRANSLATOR_H
#include "AbstractTranslator.h"

/*!
 * \brief Переводчик выражений на английский язык.
 *
 * Реализует все методы перевода абстрактного переводчика,
 * формируя текстовое описание выражения на английском языке.
 */
class EnglishTranslator : public AbstractTranslator {
public:

    /*!
     * \brief Контекст переводчика на английский язык.
     *
     * Предоставляет описания переменных и функций на основе загруженных данных.
     */
    class EnglishTranslateContext : public TranslateContext {
    public:
        /*!
         * \brief Конструктор контекста.
         * \param data Загруженные данные о переменных и функциях
         */
        EnglishTranslateContext(const TranslateContext::LoadedData& data);

        /*!
         * \brief Возвращает тип переменной по её имени.
         * \param varName Имя переменной
         * \return Строка с типом переменной
         */
        QString getVarType(const QString& varName) override;

        /*!
         * \brief Возвращает текстовое описание переменной на английском.
         * \param varName Имя переменной
         * \return Строка с описанием переменной
         */
        QString getVarDescription(const QString& varName) override;

        /*!
         * \brief Возвращает текстовое описание вызова функции на английском.
         * \param funcName Имя функции
         * \param operands Список узлов-аргументов
         * \return Строка с описанием вызова функции
         */
        QString getFuncDescription(const QString& funcName, const QVector<ExprNode*>& operands) override;
    };

    /*!
     * \brief Конструктор переводчика.
     * \param ctx Указатель на контекст переводчика
     */
    EnglishTranslator(EnglishTranslateContext* ctx);

private:
    /*!
     * \brief Определяет, нужны ли скобки вокруг операнда при переводе.
     * \param exprType Тип родительского узла
     * \param operandType Тип дочернего узла
     * \return true если скобки необходимы
     */
    bool needParentheses(ExprNode::ExprType exprType, ExprNode::ExprType operandType) override;

    QString translateSum(const QVector<QString>& parts) override;          ///< Перевод сложения на английский
    QString translateSub(const QVector<QString>& parts) override;          ///< Перевод вычитания на английский
    QString translateMultiply(const QVector<QString>& parts) override;     ///< Перевод умножения на английский
    QString translateDivide(const QVector<QString>& parts) override;       ///< Перевод деления на английский
    QString translateModulo(const QVector<QString>& parts) override;       ///< Перевод остатка от деления на английский
    QString translatePower(const QVector<QString>& parts) override;        ///< Перевод возведения в степень на английский
    QString translateUnaryMinus(const QVector<QString>& parts) override;   ///< Перевод унарного минуса на английский
    QString translateLogicalAnd(const QVector<QString>& parts) override;   ///< Перевод логического И на английский
    QString translateLogicalOr(const QVector<QString>& parts) override;    ///< Перевод логического ИЛИ на английский
    QString translateLogicalNot(ExprNode* expr, const QVector<QString>& parts) override; ///< Перевод логического НЕ на английский
    QString translateLess(const QVector<QString>& parts) override;         ///< Перевод операции меньше на английский
    QString translateGreater(const QVector<QString>& parts) override;      ///< Перевод операции больше на английский
    QString translateLessEq(const QVector<QString>& parts) override;       ///< Перевод операции меньше или равно на английский
    QString translateGreaterEq(const QVector<QString>& parts) override;    ///< Перевод операции больше или равно на английский
    QString translateEqual(const QVector<QString>& parts) override;        ///< Перевод операции равно на английский
    QString translateNotEqual(const QVector<QString>& parts) override;     ///< Перевод операции не равно на английский
    QString translateAssign(const QVector<QString>& parts) override;       ///< Перевод присваивания на английский
    QString translatePlusAssign(const QVector<QString>& parts) override;   ///< Перевод присваивания со сложением на английский
    QString translateMinusAssign(const QVector<QString>& parts) override;  ///< Перевод присваивания с вычитанием на английский
    QString translateMultAssign(const QVector<QString>& parts) override;   ///< Перевод присваивания с умножением на английский
    QString translateDivAssign(const QVector<QString>& parts) override;    ///< Перевод присваивания с делением на английский
    QString translateModAssign(const QVector<QString>& parts) override;    ///< Перевод присваивания с остатком на английский
    QString translatePreInc(const QVector<QString>& parts) override;       ///< Перевод префиксного инкремента на английский
    QString translatePostInc(const QVector<QString>& parts) override;      ///< Перевод постфиксного инкремента на английский
    QString translatePreDec(const QVector<QString>& parts) override;       ///< Перевод префиксного декремента на английский
    QString translatePostDec(const QVector<QString>& parts) override;      ///< Перевод постфиксного декремента на английский
    QString translateAddressOf(const QVector<QString>& parts) override;    ///< Перевод взятия адреса на английский
    QString translateDereference(ExprNode* expr, const QVector<QString>& parts) override; ///< Перевод разыменования на английский
    QString translateIndex(const QVector<QString>& parts) override;        ///< Перевод индексации массива на английский
};

#endif // ENGLISHTRANSLATOR_H