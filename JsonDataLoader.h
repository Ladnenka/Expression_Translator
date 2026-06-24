#ifndef JSONDATALOADER_H
#define JSONDATALOADER_H
#include "AbstractTranslator.h"
#include "Error.h"
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

/*!
 * \file JsonDataLoader.h
 * \brief Класс для загрузки данных о переменных и функциях из JSON-файлов.
 */

/*!
 * \brief Класс для загрузки данных о переменных и функциях из JSON-файлов.
 *
 * Предоставляет статические методы для чтения и валидации JSON-файлов
 * с описаниями переменных и функций.
 */
class JsonDataLoader {
public:
    /*!
     * \brief Загружает данные о переменных и функциях из JSON-файлов.
     *
     * Читает файлы по указанным путям, проверяет их корректность
     * и возвращает заполненную структуру LoadedData.
     *
     * \param[in] varsPath Путь к JSON-файлу с переменными (может быть пустым)
     * \param[in] funcsPath Путь к JSON-файлу с функциями (может быть пустым)
     * \param[out] errors Список ошибок, в который добавляются обнаруженные ошибки
     * \return Структура с загруженными переменными и функциями
     */
    static AbstractTranslator::TranslateContext::LoadedData loadFromJson(const QString& varsPath,
                                                                         const QString& funcsPath,
                                                                         QList<Error>& errors);

private:
    /*!
     * \brief Проверяет корректность имени переменной или функции.
     * \param[in] name Проверяемое имя
     * \return true если имя корректно (только буквы, цифры, подчёркивание, длина до 1000)
     */
    static bool isValidName(const QString& name);

    /*!
     * \brief Проверяет корректность строки описания.
     * \param[in] desc Проверяемое описание
     * \return true если описание корректно (допустимые символы, длина до 1000)
     */
    static bool isValidDescription(const QString& desc);

    /*!
     * \brief Проверяет, является ли тип данных поддерживаемым.
     * \param[in] type Строка с типом данных
     * \return true если тип поддерживается
     */
    static bool isSupportedType(const QString& type);

    /*!
     * \brief Проверяет наличие и корректность общих полей объекта JSON (name, description).
     * \param[in] object JSON-объект для проверки
     * \param[in] filePath Путь к файлу (для сообщений об ошибках)
     * \param[out] errors Список ошибок
     * \return true если все обязательные поля присутствуют и корректны
     */
    static bool checkCommonFields(const QJsonObject& object, const QString& filePath, QList<Error>& errors);

    /*!
     * \brief Загружает переменные из JSON-массива в структуру данных.
     * \param[in] array JSON-массив с описаниями переменных
     * \param[in] filePath Путь к файлу (для сообщений об ошибках)
     * \param[out] data Структура, в которую записываются загруженные переменные
     * \param[out] errors Список ошибок
     * \return true если загрузка прошла успешно
     */
    static bool loadVariables(const QJsonArray& array, const QString& filePath,
                              AbstractTranslator::TranslateContext::LoadedData& data, QList<Error>& errors);

    /*!
     * \brief Загружает функции из JSON-массива в структуру данных.
     * \param[in] array JSON-массив с описаниями функций
     * \param[in] filePath Путь к файлу (для сообщений об ошибках)
     * \param[out] data Структура, в которую записываются загруженные функции
     * \param[out] errors Список ошибок
     * \return true если загрузка прошла успешно
     */
    static bool loadFunctions(const QJsonArray& array, const QString& filePath,
                              AbstractTranslator::TranslateContext::LoadedData& data, QList<Error>& errors);
};

#endif // JSONDATALOADER_H
