#ifndef FUNCTION_H
#define FUNCTION_H
#include <QString>
#include <QStringList>

/*!
 * \file Function.h
 * \brief Класс функции, загружаемой из JSON-файла.
 */

/*!
 * \brief Класс, представляющий функцию с именем, параметрами и шаблоном описания.
 *
 * Используется для хранения информации о функции, загруженной из JSON-файла.
 */
class Function {
public:
    QString name;           ///< Имя функции
    QStringList paramNames; ///< Список имён параметров функции
    QStringList paramTypes; ///< Список типов параметров функции
    QString templateDesc;   ///< Шаблон текстового описания функции

    /*!
     * \brief Конструктор по умолчанию.
     */
    Function();

    /*!
     * \brief Конструктор с параметрами.
     * \param[in] n Имя функции
     * \param[in] pn Список имён параметров
     * \param[in] td Шаблон описания функции
     */
    Function(const QString& n, const QStringList& pn, const QString& td);
};

#endif // FUNCTION_H
