#ifndef FUNCTIONS_IO_H
#define FUNCTIONS_IO_H
#include "QString"
#include "QFile"
#include "Error.h"

bool loadExpressionFromFile(const QString& path, QStringList& expressions, QList<Error>& errors);
void saveResultToFile(const QString& text, const QString& filePath);
void printErrors(const QList<Error>& errors);


#endif // FUNCTIONS_IO_H
