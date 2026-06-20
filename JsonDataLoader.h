#ifndef JSONDATALOADER_H
#define JSONDATALOADER_H
#include "AbstractTranslator.h"
#include "Error.h"

class JsonDataLoader {
public:

    static AbstractTranslator::TranslateContext::LoadedData loadFromJson(const QString& varsPath,
                                                                         const QString& funcsPath,
                                                                         QList<Error>& errors);

};

#endif // JSONDATALOADER_H
