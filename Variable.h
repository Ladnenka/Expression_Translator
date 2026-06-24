#ifndef VARIABLE_H
#define VARIABLE_H
#include <QString>

/*!
 * \file Variable.h
 * \brief Класс переменной, загружаемой из JSON-файла.
 */

/*!
 * \brief Класс, представляющий переменную с именем, описанием и типом.
 *
 * Используется для хранения информации о переменной, загруженной из JSON-файла.
 */
class Variable {
public:
    QString name;        ///< Имя переменной
    QString description; ///< Описание переменной
    QString type;        ///< Тип переменной (например, int, float, bool)

    /*!
     * \brief Конструктор по умолчанию.
     */
    Variable();

    /*!
     * \brief Конструктор с параметрами.
     * \param[in] n Имя переменной
     * \param[in] d Описание переменной
     * \param[in] t Тип переменной
     */
    Variable(const QString& n, const QString& d, const QString& t);
};

#endif // VARIABLE_H
