#ifndef VARIABLE_H
#define VARIABLE_H
#include <QString>

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
     * \param n Имя переменной
     * \param d Описание переменной
     * \param t Тип переменной
     */
    Variable(const QString& n, const QString& d, const QString& t);
};

#endif // VARIABLE_H