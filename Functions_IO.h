#ifndef FUNCTIONS_IO_H
#define FUNCTIONS_IO_H
#include "QString"
#include "QFile"
#include "Error.h"

/*!
 * \file Functions_IO.h
 * \brief Функции для ввода-вывода: чтение выражений из файла и запись результатов.
 */

/*!
 * \brief Считывает выражения из текстового файла.
 *
 * Открывает файл по указанному пути, читает строки и сохраняет их в список выражений.
 * Выполняет проверки: файл существует, не пустой, содержит не более 10 выражений,
 * каждое выражение не длиннее 1000 символов.
 *
 * \param path Путь к входному файлу
 * \param expressions Список, в который будут записаны считанные выражения
 * \param errors Список ошибок, в который добавляются обнаруженные ошибки
 * \return true если файл успешно прочитан, false если возникла ошибка
 */
bool loadExpressionFromFile(const QString& path, QStringList& expressions, QList<Error>& errors);

/*!
 * \brief Сохраняет результат перевода в текстовый файл.
 *
 * Создаёт файл по указанному пути или перезаписывает существующий.
 * Записывает переданный текст в файл в кодировке UTF-8.
 *
 * \param text Текст для записи в файл
 * \param filePath Путь к выходному файлу
 */
void saveResultToFile(const QString& text, const QString& filePath);

/*!
 * \brief Выводит список ошибок в стандартный вывод.
 *
 * Для каждой ошибки из списка вызывает generateErrorMessage() и выводит результат через printf.
 *
 * \param errors Список ошибок для вывода
 */
void printErrors(const QList<Error>& errors);

#endif // FUNCTIONS_IO_H