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
        loadVariables(document.array(), varsPath, data, errors);
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
        loadFunctions(document.array(), funcsPath, data, errors);
    }
    //Возвратить заполненную структуру data
    return data;
}

bool JsonDataLoader::loadVariables(const QJsonArray& array, const QString& filePath,
                                   AbstractTranslator::TranslateContext::LoadedData& data, QList<Error>& errors) {
    QSet<QString> alreadyUsedNames;
    bool hasErrors = false;

    for (int i = 0; i < array.size(); i++) {
        QJsonObject object = array[i].toObject();
        bool objectHasError = false;
        QString objNum = QString::number(i + 1);

        //проверяем name и description
        if (!checkCommonFields(object, objNum, filePath, errors)) {
            objectHasError = true;
            hasErrors = true;
        }

        //если объект содержит поле parameters, то это функция, а не переменная
        if (object.contains("parameters")) {
            errors.append(Error(Error::MissingRequiredField, objNum, "type", filePath, -1, -1, object["name"].toString()));
            objectHasError = true;
            hasErrors = true;
        }
        //нет поля type
        else if (!object.contains("type")) {
            errors.append(Error(Error::MissingRequiredField, objNum, "type", filePath, -1, -1, object["name"].toString()));
            objectHasError = true;
            hasErrors = true;
        } else {
            QString name = object["name"].toString();
            QString type = object["type"].toString();

            //неподдерживаемый тип
            if (!isSupportedType(type)) {
                errors.append(Error(Error::UnsupportedType, objNum, type, filePath, -1, -1, name));
                objectHasError = true;
                hasErrors = true;
            }
        }

        //дубликат переменной — проверяем только если объект без ошибок
        if (!objectHasError) {
            QString name = object["name"].toString();
            if (alreadyUsedNames.contains(name)) {
                errors.append(Error(Error::DuplicateVariable, objNum, name, filePath));
                hasErrors = true;
            } else {
                alreadyUsedNames.insert(name);
                data.variables.append(Variable(name, object["description"].toString(), object["type"].toString()));
            }
        }
    }

    return !hasErrors;
}

bool JsonDataLoader::loadFunctions(const QJsonArray& array, const QString& filePath,
                                   AbstractTranslator::TranslateContext::LoadedData& data, QList<Error>& errors) {
    QSet<QString> alreadyUsedSignatures;
    bool hasErrors = false;

    for (int i = 0; i < array.size(); i++) {
        QJsonObject object = array[i].toObject();
        bool objectHasError = false;
        QString objNum = QString::number(i + 1);

        if (!checkCommonFields(object, objNum, filePath, errors)) {
            objectHasError = true;
            hasErrors = true;
        }

        if (!object.contains("parameters")) {
            errors.append(Error(Error::MissingRequiredField, objNum, "parameters", filePath, -1, -1, object["name"].toString()));
            objectHasError = true;
            hasErrors = true;
        } else {
            QJsonArray parametersArray = object["parameters"].toArray();
            QStringList paramNames;
            QStringList paramTypes;
            bool paramsHaveError = false;

            for (int j = 0; j < parametersArray.size(); j++) {
                QJsonObject param = parametersArray[j].toObject();
                //Номер параметра для сообщений об ошибках: "2.1", "2.2" и т.д.
                QString paramNum = objNum + "." + QString::number(j + 1);

                if (!param.contains("name")) {
                    errors.append(Error(Error::MissingRequiredField, paramNum, "name", filePath, -1, -1, object["name"].toString()));
                    paramsHaveError = true;
                    hasErrors = true;
                }

                if (!param.contains("type")) {
                    errors.append(Error(Error::MissingRequiredField, paramNum, "type", filePath, -1, -1, object["name"].toString()));
                    paramsHaveError = true;
                    hasErrors = true;
                } else {
                    QString pType = param["type"].toString();
                    if (!isSupportedType(pType)) {
                        errors.append(Error(Error::UnsupportedType, paramNum, pType, filePath, -1, -1, pType));
                        paramsHaveError = true;
                        hasErrors = true;
                    }
                }

                if (!paramsHaveError) {
                    paramNames.append(param["name"].toString());
                    paramTypes.append(param["type"].toString());
                }
                paramsHaveError = false;
            }

            //дубликат функции — проверяем только если объект без ошибок
            if (!objectHasError) {
                QString name = object["name"].toString();
                QString signature = name + "(" + paramTypes.join(",") + ")";
                if (alreadyUsedSignatures.contains(signature)) {
                    errors.append(Error(Error::DuplicateFunction, objNum, name, filePath));
                    hasErrors = true;
                } else {
                    alreadyUsedSignatures.insert(signature);
                    data.functions.append(Function(name, paramNames, object["description"].toString()));
                    data.functionNames.append(name);
                    data.functionArgCount[name] = paramNames.size();
                }
            }
        }
    }

    return !hasErrors;
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
bool JsonDataLoader::checkCommonFields(const QJsonObject& object, const QString& objNum,
                                       const QString& filePath, QList<Error>& errors) {
    bool hasErrors = false;

    //Если объект не содержит поля "name"
    if (!object.contains("name")) {
        //Добавить соответствующую ошибку в массив ошибок и завершить выполнение метода
        errors.append(Error(Error::MissingRequiredField, objNum, "name", filePath));
        //Имя неизвестно — дальнейшие проверки description бессмысленны
        return false;
    }

    //Извлечь значение поля "name" и сохранить как строку имени
    QString name = object["name"].toString();

    //Если строка имени пустая
    if (name.isEmpty()) {
        //Добавить соответствующую ошибку в массив ошибок
        errors.append(Error(Error::EmptyVariableName, objNum, "", filePath));
        hasErrors = true;
    }
    //Если строка имени содержит недопустимые символы
    else if (!isValidName(name)) {
        //Добавить в список ошибок соответствующую ошибку
        errors.append(Error(Error::InvalidCharacters, objNum, "name", filePath, -1, -1, name));
        hasErrors = true;
    }

    //Проверить наличие описания у объекта
    if (!object.contains("description")) {
        errors.append(Error(Error::MissingRequiredField, objNum, "description", filePath, -1, -1, name));
        hasErrors = true;
    } else {
        //Извлечь значение поля "description" и проверить его на валидность
        //Если описание содержит недопустимые символы
        QString description = object["description"].toString();
        if (!isValidDescription(description)) {
            //Добавить в список ошибок соответствующую ошибку
            errors.append(Error(Error::InvalidCharacters, objNum, "description", filePath, -1, -1, description));
            hasErrors = true;
        }
    }

    //Вернуть признак пройденных проверок
    return !hasErrors;
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
