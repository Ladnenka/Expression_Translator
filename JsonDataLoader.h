#ifndef JSONDATALOADER_H
#define JSONDATALOADER_H
#include "AbstractTranslator.h"
#include "Error.h"
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

class JsonDataLoader {
public:

    static AbstractTranslator::TranslateContext::LoadedData loadFromJson(const QString& varsPath,
                                                                         const QString& funcsPath,
                                                                         QList<Error>& errors);

private:

    static bool loadVariables(const QJsonArray& array, const QString& filePath,
                              AbstractTranslator::TranslateContext::LoadedData& data, QList<Error>& errors);

    static bool loadFunctions(const QJsonArray& array, const QString& filePath,
                              AbstractTranslator::TranslateContext::LoadedData& data, QList<Error>& errors);
};

#endif // JSONDATALOADER_H
