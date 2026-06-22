#ifndef FUNCTION_H
#define FUNCTION_H
#include <QString>
#include <QStringList>

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
     * \param n Имя функции
     * \param pn Список имён параметров
     * \param td Шаблон описания функции
     */
    Function(const QString& n, const QStringList& pn, const QString& td);
};

#endif // FUNCTION_H