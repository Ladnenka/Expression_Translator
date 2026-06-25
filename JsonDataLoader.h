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
 *
 * Предоставляет статические методы для чтения и валидации JSON-файлов
 * с описаниями переменных и функций.
 */

/*!
 * \brief Kласс для загрузки данных о переменных и функциях из JSON-файлов.
 *
 */
class JsonDataLoader {
public:
    /*!
     * \brief Загружает данные о переменных и функциях из JSON-файлов.
     *
     * Читает файлы по указанным путям, проверяет их корректность
     * и возвращает заполненную структуру LoadedData.
     * Накапливает все ошибки — не останавливается на первой.
     * Единственный ранний выход — если файл не удалось открыть.
     *
     * \param varsPath Путь к JSON-файлу с переменными (может быть пустым)
     * \param funcsPath Путь к JSON-файлу с функциями (может быть пустым)
     * \param errors Список ошибок, в который добавляются обнаруженные ошибки
     * \return Структура с загруженными переменными и функциями
     */
    static AbstractTranslator::TranslateContext::LoadedData loadFromJson(const QString& varsPath,
                                                                         const QString& funcsPath,
                                                                         QList<Error>& errors);

private:
    /*!
     * \brief Проверяет корректность имени переменной или функции.
     * \param name Проверяемое имя
     * \return true если имя корректно (только буквы, цифры, подчёркивание, длина до 1000)
     */
    static bool isValidName(const QString& name);

    /*!
     * \brief Проверяет корректность строки описания.
     * \param desc Проверяемое описание
     * \return true если описание корректно (допустимые символы, длина до 1000)
     */
    static bool isValidDescription(const QString& desc);

    /*!
     * \brief Проверяет, является ли тип данных поддерживаемым.
     * \param type Строка с типом данных
     * \return true если тип поддерживается
     */
    static bool isSupportedType(const QString& type);

    /*!
     * \brief Проверяет наличие и корректность общих полей объекта JSON (name, description).
     *
     * Если поле name отсутствует — ранний выход, дальнейшие проверки бессмысленны.
     * Иначе накапливает все ошибки по name и description.
     *
     * \param object JSON-объект для проверки
     * \param objNum Номер объекта в массиве (1-based, для сообщений об ошибках)
     * \param filePath Путь к файлу (для сообщений об ошибках)
     * \param errors Список ошибок
     * \return true если все обязательные поля присутствуют и корректны
     */
    static bool checkCommonFields(const QJsonObject& object, const QString& objNum,
                                  const QString& filePath, QList<Error>& errors);

    /*!
     * \brief Загружает переменные из JSON-массива в структуру данных.
     *
     * Накапливает все ошибки по всем объектам, не останавливаясь на первой.
     *
     * \param array JSON-массив с описаниями переменных
     * \param filePath Путь к файлу (для сообщений об ошибках)
     * \param data Структура, в которую записываются загруженные переменные
     * \param errors Список ошибок
     * \return true если загрузка прошла без ошибок
     */
    static bool loadVariables(const QJsonArray& array, const QString& filePath,
                              AbstractTranslator::TranslateContext::LoadedData& data,
                              QList<Error>& errors);

    /*!
     * \brief Загружает функции из JSON-массива в структуру данных.
     *
     * Накапливает все ошибки по всем объектам, не останавливаясь на первой.
     * Номера параметров выводятся в формате "объект.параметр" (например "2.1").
     *
     * \param array JSON-массив с описаниями функций
     * \param filePath Путь к файлу (для сообщений об ошибках)
     * \param data Структура, в которую записываются загруженные функции
     * \param errors Список ошибок
     * \return true если загрузка прошла без ошибок
     */
    static bool loadFunctions(const QJsonArray& array, const QString& filePath,
                              AbstractTranslator::TranslateContext::LoadedData& data,
                              QList<Error>& errors);
};

#endif // JSONDATALOADER_H
