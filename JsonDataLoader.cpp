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
    QSet<QString> alreadyUsedNames;

    for (int i = 0; i < array.size(); i++) {
        QJsonObject object = array[i].toObject();

        //проверяем name и description
        if (!checkCommonFields(object, filePath, errors)) return false;

        QString name = object["name"].toString();

        //нет поля type
        if (!object.contains("type")) {
            errors.append(Error(Error::MissingRequiredField, "type", "", filePath));
            return false;
        }

        QString type = object["type"].toString();

        //неподдерживаемый тип
        if (!isSupportedType(type)) {
            errors.append(Error(Error::UnsupportedType, name, type, filePath));
            return false;
        }

        //дубликат переменной
        if (alreadyUsedNames.contains(name)) {
            errors.append(Error(Error::DuplicateVariable, name, "", filePath));
            return false;
        }

        alreadyUsedNames.insert(name);
        data.variables.append(Variable(name, object["description"].toString(), type));
    }

    return true;
}

bool JsonDataLoader::loadFunctions(const QJsonArray& array, const QString& filePath,
                                   AbstractTranslator::TranslateContext::LoadedData& data, QList<Error>& errors) {
    QSet<QString> alreadyUsedSignatures;

    for (int i = 0; i < array.size(); i++) {
        QJsonObject object = array[i].toObject();

        if (!checkCommonFields(object, filePath, errors)) return false;

        QString name = object["name"].toString();

        if (!object.contains("parameters")) {
            errors.append(Error(Error::MissingRequiredField, "parameters", "", filePath));
            return false;
        }

        QJsonArray parametersArray = object["parameters"].toArray();
        QStringList paramNames; QStringList paramTypes;

        for (int j = 0; j < parametersArray.size(); j++) {
            QJsonObject param = parametersArray[j].toObject();

            if (!param.contains("name")) {
                errors.append(Error(Error::MissingRequiredField, "name", "", filePath));
                return false;
            }

            if (!param.contains("type")) {
                errors.append(Error(Error::MissingRequiredField, "type", "", filePath));
                return false;
            }

            QString pName = param["name"].toString(); QString pType = param["type"].toString();

            if (!isSupportedType(pType)) {
                errors.append(Error(Error::UnsupportedType, pType, "", filePath));
            }

            paramNames.append(pName);
            paramTypes.append(pType);
        }

        QString signature = name + "(" + paramTypes.join(",") + ")";

        if (alreadyUsedSignatures.contains(signature)) {
            errors.append(Error(Error::DuplicateFunction, name, "", filePath));
            return false;
        }

        alreadyUsedSignatures.insert(signature);
        data.functions.append(Function(name, paramNames, object["description"].toString()));
        data.functionNames.append(name);
        data.functionArgCount[name] = paramNames.size();
    }
    return true;
}

bool JsonDataLoader::isSupportedType(const QString& type) {
    static const QStringList types = {"int", "float", "double", "char", "bool", "short", "unsigned int"};
    if (types.contains(type)) return true;
    if (type.endsWith("*"))
        return types.contains(type.left(type.length() - 1).trimmed());
    if (type.endsWith("[]"))
        return types.contains(type.left(type.length() - 2).trimmed());
    return false;
}

// Проверка общих полей для переменной и функции: name, description
bool JsonDataLoader::checkCommonFields(const QJsonObject& object, const QString& filePath, QList<Error>& errors) {
    //Если объект не содержит поля "name"
    if (!object.contains("name")) {
        //Добавить соответствующую ошибку в массив ошибок и завершить выполнение метода
        errors.append(Error(Error::MissingRequiredField, "name", "", filePath));
        return false;
    }

    //Извлечь значение поля "name" и сохранить как строку имени
    QString name = object["name"].toString();

    //Если строка имени пустая
    if (name.isEmpty()) {
        //Добавить соответствующую ошибку в массив ошибок и завершить выполнение метода
        errors.append(Error(Error::EmptyVariableName, "", "", filePath));
        return false;
    }

    //Если строка имени содержит недопустимые символы
    if (!isValidName(name)) {
        //Добавить в список ошибок соответствующую ошибку и завершить выполнение метода
        errors.append(Error(Error::InvalidCharacters, name, "name", filePath));
        return false;
    }

    //Проверить наличие описания у объекта
    if (!object.contains("description")) {
        errors.append(Error(Error::MissingRequiredField, "description", "", filePath));
        return false;
    }

    //Извлечь значение поля "description" и проверить его на валидность
    //Если описание содержит недопустимые символы
    QString description = object["description"].toString();
    if (!isValidDescription(description)) {
        //Добавить в список ошибок ошибку соответствующую ошибку и завершить выполнение метода
        errors.append(Error(Error::InvalidCharacters, name, "description", filePath));
        return false;
    }
    //Вернуть признак пройденных проверок
    return true;
}

bool JsonDataLoader::isValidName(const QString& name) {
    if (name.length() > 1000) return false;
    for (const QChar& ch : name)
        if (!ch.isLetterOrNumber() && ch != '_') return false;
    return true;
}

bool JsonDataLoader::isValidDescription(const QString& desc) {
    if (desc.length() > 1000) return false;
    for (const QChar& ch : desc) {
        if (ch.isLetterOrNumber() || ch == ' ' || ch == '{' || ch == '}' || ch == '-' || ch == ',') continue;
        return false;
    }
    return true;
}
