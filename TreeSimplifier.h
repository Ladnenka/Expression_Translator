#ifndef TREESIMPLIFIER_H
#define TREESIMPLIFIER_H
#include "ExprNode.h"

/*!
 * \file TreeSimplifier.h
 * \brief Класс для упрощения дерева выражения.
 *
 * Предоставляет статические методы для рекурсивного обхода и упрощения
 * дерева выражения. Поддерживает упрощение сложения, вычитания, умножения,
 * деления, операций с указателями и двойного унарного минуса.
 */
class TreeSimplifier {
public:
    /*!
     * \brief Рекурсивно упрощает дерево выражения начиная с переданного узла.
     *
     * Сначала рекурсивно упрощает все дочерние узлы,
     * затем применяет соответствующее упрощение к текущему узлу.
     *
     * \param [in] root Ссылка на указатель корневого узла (может быть заменён)
     */
    static void simplify(ExprNode*& root);

private:
    /*!
     * \brief Создаёт узел с отрицательным значением операнда.
     *
     * Если операнд уже является унарным минусом — убирает двойное отрицание.
     * Иначе — оборачивает операнд в новый узел UNARY_MINUS.
     *
     * \param [in] op Узел операнда
     * \return Новый узел с отрицательным значением
     */
    static ExprNode* makeNegative(ExprNode* op);

    /*!
     * \brief Упрощает узел сложения.
     *
     * Раскрывает вложенные узлы PLUS и MINUS:
     * вложенные плюсы расплющиваются, вычитания преобразуются в сложение с отрицательным операндом.
     *
     * \param [in] root Узел типа PLUS
     */
    static void simplifyPlus(ExprNode* root);

    /*!
     * \brief Упрощает узел вычитания.
     *
     * Обрабатывает случаи когда левый или правый операнд является PLUS или MINUS,
     * преобразуя выражение к более простой форме.
     *
     * \param [in] root Узел типа MINUS
     */
    static void simplifyMinus(ExprNode* root);

    /*!
     * \brief Упрощает узел умножения.
     *
     * Раскрывает вложенные узлы MULTIPLY, собирая все множители в плоский список.
     *
     * \param [in] root Узел типа MULTIPLY
     */
    static void simplifyMultiply(ExprNode* root);

    /*!
     * \brief Упрощает узел деления.
     *
     * Обрабатывает случаи когда левый или правый операнд является DIVIDE,
     * устраняя вложенные деления путём перестановки операндов.
     *
     * \param [in] root Узел типа DIVIDE
     */
    static void simplifyDivide(ExprNode* root);

    /*!
     * \brief Упрощает операции с указателями.
     *
     * Устраняет взаимно обратные операции: *(&x) → x и &(*x) → x.
     *
     * \param [in] root Ссылка на указатель узла типа ADDRESS_OF или DEREFERENCE (может быть заменён)
     */
    static void simplifyPointer(ExprNode*& root);

    /*!
     * \brief Упрощает двойной унарный минус.
     *
     * Устраняет двойное отрицание: -(-x) → x.
     *
     * \param [in] root Ссылка на указатель узла типа UNARY_MINUS (может быть заменён)
     */
    static void simplifyUnaryMinus(ExprNode*& root);
};

#endif // TREESIMPLIFIER_H
