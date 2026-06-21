#include "JsonDataLoader.h"

AbstractTranslator::TranslateContext::LoadedData
JsonDataLoader::loadFromJson(const QString& varsPath, const QString& funcsPath, QList<Error>& errors) {
    //Создать структуру LoadedData для хранения данных с файлов JSON о функциях и переменных
    AbstractTranslator::TranslateContext::LoadedData data;

    //Если путь к файлу переменных не пустой
    if (!varsPath.isEmpty()) {
        //Открыть файл переменных по указанному пути
        QFile file(varsPath);
        //Если файл не существует или не удалось открыть
        if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
            //Добавить соответствующую ошибку в массив ошибок
            errors.append(Error(Error::FileNotExist, "", "", varsPath));
            return data;
        }

        //Прочитать содержимое файла и разобрать как JSON-документ, а затем закрыть файл
        QJsonDocument document = QJsonDocument::fromJson(file.readAll());
        file.close();

        //Если JSON-документ не является массивом
        if (!document.isArray()) {
            //Добавить соответствующую ошибку в массив ошибок
            errors.append(Error(Error::JsonParseError, "", "", varsPath));
            return data;
        }

        //Вызвать метод загрузки переменных
        if (!loadVariables(document.array(), varsPath, data, errors)) {
            return data;
        }
    }

    //Если путь к файлу функций не пустой
    if (!funcsPath.isEmpty()) {
        //Открыть файл по указанному пути
        QFile file(funcsPath);
        //Если файл не существует или не удалось открыть
        if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
            //Добавить соответствующую ошибку в массив ошибок
            errors.append(Error(Error::FileNotExist, "", "", funcsPath));
            return data;
        }

        //Прочитать содержимое файла и разобрать как JSON-документ, а затем закрыть файл
        QJsonDocument document = QJsonDocument::fromJson(file.readAll());
        file.close();

        //Если JSON-документ не является массивом
        if (!document.isArray()) {
            //Добавить соответствующую ошибку в массив ошибок
            errors.append(Error(Error::JsonParseError, "", "", funcsPath));
            return data;
        }

        //Вызвать метод загрузки функций
        if (!loadFunctions(document.array(), funcsPath, data, errors)) {
            return data;
        }
    }
    //Возвратить заполненную структуру data
    return data;
}

bool JsonDataLoader::loadVariables(const QJsonArray& array, const QString& filePath,
                                   AbstractTranslator::TranslateContext::LoadedData& data, QList<Error>& errors) {
    return true;
}

bool JsonDataLoader::loadFunctions(const QJsonArray& array, const QString& filePath,
                                   AbstractTranslator::TranslateContext::LoadedData& data, QList<Error>& errors) {
    return true;
}
