#ifndef ERROR_H
#define ERROR_H
#include <QString>

/*!
 * \file Error.h
 * \brief Класс для представления диагностической ошибки.
 */

/*!
 * \brief Класс для представления ошибки, возникшей при обработке выражения или загрузке данных.
 *
 * Хранит тип ошибки и дополнительную информацию: идентификатор, тип данных,
 * путь к файлу, позицию в строке.
 */
class Error {
public:
    /*!
     * \brief Перечисление типов ошибок.
     */
    enum ErrorType {
        UnknownFunction,      ///< Неизвестная функция или переменная
        NotEnoughOperands,    ///< Недостаточно операндов для операции
        ExtraOperands,        ///< Лишние операнды остались в стеке
        UnsupportedOperation, ///< Неподдерживаемая операция
        InvalidArgumentCount, ///< Неверное количество аргументов функции
        FileNotExist,         ///< Файл не существует или недоступен
        FileNotRead,          ///< Файл не удалось прочитать
        JsonParseError,       ///< Ошибка разбора JSON
        MissingRequiredField, ///< Отсутствует обязательное поле в JSON
        UnsupportedType,      ///< Неподдерживаемый тип данных
        EmptyVariableName,    ///< Пустое имя переменной или функции
        InvalidCharacters,    ///< Недопустимые символы в имени или описании
        DuplicateVariable,    ///< Дублирование имени переменной
        DuplicateFunction,    ///< Дублирование сигнатуры функции
        ExpressionTooLong,    ///< Выражение превышает допустимую длину
        TooManyOperations,    ///< Слишком много операций в выражении
        EmptyInputFile,       ///< Входной файл пустой
        MissingArguments,     ///< Недостаточно аргументов командной строки
        TooManyExpressions,   ///< Слишком много выражений во входном файле
        SideEffectConflict    ///< Конфликт побочных эффектов (двойное изменение переменной)
    };

    ErrorType type;       ///< Тип ошибки
    QString id;           ///< Номер объекта в JSON (1-based) или идентификатор для не-JSON ошибок
    QString dataTypeStr;  ///< Имя проблемного поля или тип данных
    QString extraInfo;    ///< Дополнительная информация (невалидное значение, имя переменной и т.д.)
    QString errorFilePath;///< Путь к файлу, в котором возникла ошибка
    int errorColumn;      ///< Номер столбца (позиция в строке)
    int errorLine;        ///< Номер строки или позиция в выражении

    /*!
     * \brief Конструктор ошибки.
     * \param[in] t Тип ошибки
     * \param[in] identifier Номер объекта в JSON или идентификатор
     * \param[in] dataType Имя проблемного поля или тип данных
     * \param[in] filePath Путь к файлу
     * \param[in] column Номер столбца
     * \param[in] line Номер строки или позиция в выражении
     * \param[in] extra Дополнительная информация
     */
    explicit Error(ErrorType t, const QString& identifier = "", const QString& dataType = "",
                   const QString& filePath = "", int column = -1, int line = -1,
                   const QString& extra = "");

    /*!
     * \brief Генерирует текстовое сообщение об ошибке.
     * \return Строка с описанием ошибки
     */
    QString generateErrorMessage() const;
};

#endif // ERROR_H
