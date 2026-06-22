#ifndef TREEBUILDER_H
#define TREEBUILDER_H
#include "ExprNode.h"
#include <QStack>
#include <QSet>
#include "AbstractTranslator.h"
#include "Error.h"

/*!
 * \brief Класс для построения дерева выражения из постфиксной записи.
 *
 * Обрабатывает список токенов постфиксного выражения и строит
 * соответствующее дерево узлов ExprNode с помощью стека.
 */
class TreeBuilder {
public:
    QStack<ExprNode*> stack;       ///< Стек узлов для построения дерева
    QSet<QString> modifiedVariables; ///< Множество переменных, изменённых в выражении (для проверки побочных эффектов)

    /*!
     * \brief Определяет, является ли токен числом.
     * \param token Строка токена
     * \return true если токен является числом
     */
    bool isNumberToken(const QString& token);

    /*!
     * \brief Строит узел бинарной операции из двух верхних элементов стека.
     *
     * Извлекает два узла из стека, создаёт узел операции и помещает его обратно.
     *
     * \param opType Тип операции
     * \param position Позиция токена в строке выражения
     * \param errors Список ошибок
     * \return true если операция выполнена успешно
     */
    bool buildBinaryOp(ExprNode::ExprType opType, int position, QList<Error>& errors);

    /*!
     * \brief Строит узел унарной операции из верхнего элемента стека.
     *
     * Извлекает один узел из стека, создаёт узел операции и помещает его обратно.
     *
     * \param opType Тип операции
     * \param position Позиция токена в строке выражения
     * \param errors Список ошибок
     * \return true если операция выполнена успешно
     */
    bool buildUnaryOp(ExprNode::ExprType opType, int position, QList<Error>& errors);

    /*!
     * \brief Строит узел n-арной операции с автоматическим раскрытием вложенных операций того же типа.
     *
     * Извлекает два узла из стека. Если левый или правый узел имеет тот же тип,
     * их дочерние узлы переносятся напрямую (расплющивание дерева).
     *
     * \param opType Тип операции
     * \param position Позиция токена в строке выражения
     * \param errors Список ошибок
     * \return true если операция выполнена успешно
     */
    bool buildNAryOp(ExprNode::ExprType opType, int position, QList<Error>& errors);

    /*!
     * \brief Строит узел вызова функции.
     *
     * Извлекает имя функции из стека, проверяет её наличие в загруженных данных,
     * извлекает нужное количество аргументов и создаёт узел FUNCTION.
     *
     * \param position Позиция токена в строке выражения
     * \param data Загруженные данные о функциях
     * \param errors Список ошибок
     * \return true если вызов функции построен успешно
     */
    bool buildFunctionCall(int position, const AbstractTranslator::TranslateContext::LoadedData& data, QList<Error>& errors);

    /*!
     * \brief Строит дерево выражения из списка токенов постфиксной записи.
     *
     * Обрабатывает каждый токен: числа и переменные помещаются в стек,
     * операторы извлекают операнды и создают узлы операций.
     * По завершении в стеке должен остаться ровно один узел — корень дерева.
     *
     * \param tokens Список токенов постфиксного выражения
     * \param originalExpr Исходная строка выражения (для определения позиций)
     * \param data Загруженные данные о переменных и функциях
     * \param errors Список ошибок
     * \return Указатель на корневой узел дерева, или nullptr при ошибке
     */
    ExprNode* buildTree(const QStringList& tokens,
                        const QString& originalExpr,
                        const AbstractTranslator::TranslateContext::LoadedData& data,
                        QList<Error>& errors);

    /*!
     * \brief Деструктор. Очищает стек и удаляет оставшиеся узлы.
     */
    ~TreeBuilder();

private:
    /*!
     * \brief Проверяет, не изменяется ли переменная повторно в одном выражении.
     *
     * Используется при обработке операций с побочным эффектом (инкремент, декремент).
     * Если переменная уже была изменена ранее — добавляет ошибку SideEffectConflict.
     *
     * \param operand Узел операнда для проверки
     * \param position Позиция токена в строке выражения
     * \param errors Список ошибок
     * \return true если конфликта побочных эффектов нет
     */
    bool checkSideEffect(ExprNode* operand, int position, QList<Error>& errors);
};

#endif // TREEBUILDER_H