#include "Functions_IO.h"

//Метод считывания выражений с файла
bool loadExpressionFromFile(const QString& path, QStringList& expressions, QList<Error>& errors) {
    QFile file(path);
    //Если путь пустой или файл не удаётся открыть
    if (path.isEmpty() || !file.open(QIODevice::ReadOnly)) {
        //Сохранить соответствующую ошибку в массив ошибок
        errors.append(Error(Error::FileNotExist, "", "", path));
        return false;
    }
    //Прочитать содержимое файла
    QString fullContent = QString::fromUtf8(file.readAll()).trimmed();
    file.close();
    //Если файл пустой
    if (fullContent.isEmpty()) {
        //Сохранить соответствующую ошибку в массив ошибок
        errors.append(Error(Error::EmptyInputFile, "", "", path));
        return false;
    }
    //Разбить содержимое на строки и отфильтровать пустые
    QStringList allLines = fullContent.split('\n');
    expressions.clear();
    for (const QString& line : allLines) {
        QString trimmedLine = line.trimmed();
        if (!trimmedLine.isEmpty()) {
            expressions.append(trimmedLine);
        }
    }
    //Если количество выражений превышает допустимое
    if (expressions.size() > 10) {
        //Сохранить соответствующую ошибку в массив ошибок
        errors.append(Error(Error::TooManyExpressions, "", "", ""));
        expressions.clear();
        return false;
    }
    //Проверить длину каждого выражения
    for (int i = 0; i < expressions.size(); ++i) {
        //Если длина выражения превышает допустимую
        if (expressions[i].length() > 1000) {
            //Сохранить соответствующую ошибку в массив ошибок
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
