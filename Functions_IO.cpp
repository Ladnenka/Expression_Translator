#include "Functions_IO.h"

//Метод считывания выражений с файла
bool loadExpressionFromFile(const QString& path, QStringList& expressions, QList<Error>& errors) {
    QFile file(path);

    if (path.isEmpty() || !file.open(QIODevice::ReadOnly)) {
        errors.append(Error(Error::FileNotExist, "", "", path));
        return false;
    }

    QString fullContent = QString::fromUtf8(file.readAll()).trimmed();
    file.close();

    if (fullContent.isEmpty()) {
        errors.append(Error(Error::EmptyInputFile, "", "", path));
        return false;
    }

    expressions = fullContent.split('\n', Qt::SkipEmptyParts);

    if (expressions.size() > 10) {
        errors.append(Error(Error::TooManyExpressions, "", "", ""));
        return false;
    }

    for (int i = 0; i < expressions.size(); ++i) {
        if (expressions[i].trimmed().length() > 1000) {
            errors.append(Error(Error::ExpressionTooLong, "", "", path));
            expressions.clear();
            return false;
        }
    }

    return true;
}

//Метод записи перевода выражения в файл
void saveResultToFile(const QString& text, const QString& filePath) {
    QFile outFile(filePath);
    //Перезаписывает содержимое существующего файла или создаёт новый, если файл не существует
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        outFile.write(text.toUtf8());
        outFile.close();
        printf("\nResult saved to %s\n", qPrintable(filePath));
    }
}

void printErrors(const QList<Error>& errors) {
    for (const Error& err : errors) {
        printf("%s\n", qPrintable(err.generateErrorMessage()));
    }
}
