#include "../Error.h"

Error::Error(ErrorType t, const QString& identifier, const QString& dataType,
             const QString& filePath, int column, int line)
    : type(t), id(identifier), dataTypeStr(dataType), errorFilePath(filePath),
    errorColumn(column), errorLine(line) {}
