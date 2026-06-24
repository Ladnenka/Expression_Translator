/*!
 * \file MAIN.cpp
 * \brief Данный файл содержит главную функцию программы Expression Translator.
 *
 * \mainpage Документация для программы "Expression Translator"
 *
 * Программа предназначена для перевода выражений на языке C++,
 * записанных в постфиксной нотации, на английский язык.
 * Для функционирования программы необходима операционная система Windows 7 или выше.
 * Программа разработана на языке C++ с использованием библиотеки Qt
 * в среде разработки Qt Creator.
 * Программа должна получать минимум два аргумента командной строки:
 * имя входного файла и имя выходного файла в формате 'txt'.
 *
 * Пример команды запуска программы:
 * \code
 * run.bat Test_InputData/input.txt Test_InputData/output.txt
 * \endcode
 *
 * \author Ivanov R.S.
 * \date June 2026
 * \version 1.0
 */

#include <QCoreApplication>
#include <cstdio>
#include "Error.h"
#include "Functions_IO.h"
#include "JsonDataLoader.h"
#include "TreeBuilder.h"
#include "TreeSimplifier.h"
#include "EnglishTranslator.h"

/*!
 * \brief Главная функция программы.
 * \param[in] argc Количество переданных аргументов командной строки
 * \param[in] argv Аргументы командной строки:
 *   - argv[0] — аргумент запуска программы
 *   - argv[1] — путь к входному файлу
 *   - argv[2] — путь к выходному файлу
 *   - argv[3] — путь к JSON-файлу с переменными (необязательно)
 *   - argv[4] — путь к JSON-файлу с функциями (необязательно)
 * \return 0 если программа завершилась без ошибок, 1 при наличии ошибок
 */
int main(int argc, char *argv[]) {
    QList<Error> errors;

    //Проверить аргументы командной строки (argc)
    //Если аргументов меньше трёх
    if (argc < 3) {
        //Вывести ошибку об отсутствии минимального количества аргументов
        errors.append(Error(Error::MissingArguments, "", "", "", argc - 1));
        printf("\nCOMPILATION ERRORS FOUND\n");
        printErrors(errors);
        return 1;
    }

    //Считать из параметров командной строки все пути к файлам
    QString inputPath  = QString::fromLocal8Bit(argv[1]);
    QString outputPath = QString::fromLocal8Bit(argv[2]);
    QString varsPath   = argc >= 4 ? QString::fromLocal8Bit(argv[3]) : "";
    QString funcsPath  = argc >= 5 ? QString::fromLocal8Bit(argv[4]) : "";

    //Если не удалось прочитать файл с выражениями для перевода
    QStringList expressions;
    if (!loadExpressionFromFile(inputPath, expressions, errors)) {
        //Вывести ошибку об прочтении файла
        printf("\nCOMPILATION ERRORS FOUND\n");
        printErrors(errors);
        return 1;
    }

    //Загрузить переменные и функции из Json файлов в структуру данных
    auto data = JsonDataLoader::loadFromJson(varsPath, funcsPath, errors);

    //Если возникли ошибки при загрузке
    if (!errors.isEmpty()) {
        //Вывести ошибку о прочтении данных
        printf("\nJSON LOADING ERRORS\n");
        printErrors(errors);
        return 1;
    }

    QStringList totalResults;
    bool globalError = false;

    //Для каждого выражения из входного файла с выражениями
    for (int i = 0; i < expressions.size(); ++i) {
        QString expression = expressions[i].trimmed();
        if (expression.isEmpty()) continue;

        printf("\n[Processing Expression %d/%d]: %s\n", i + 1, expressions.size(), qPrintable(expression));

        //Построить дерево выражения
        QList<Error> localErrors;
        TreeBuilder builder;

        //Получить корень дерева
        ExprNode* root = builder.buildTree(expression.split(" ", Qt::SkipEmptyParts), expression, data, localErrors);

        //Если корень пустой или возникла ошибка при построении дерева
        if (!root || !localErrors.isEmpty()) {
            //Записать соответствующую ошибку в массив ошибок
            printf("ERRORS FOUND IN EXPRESSION %d:\n", i + 1);
            QStringList lineErrorMessages;
            for (const Error& err : localErrors) {
                QString msg = err.generateErrorMessage();
                printf("   %s\n", qPrintable(msg));
                lineErrorMessages.append(msg);
            }
            totalResults.append(QString("[ERROR IN LINE %1: %2]").arg(i + 1).arg(lineErrorMessages.join("; ")));
            //Завершить итерацию цикла
            if (root) delete root;
            globalError = true;
            continue;
        }

        //Упростить дерево выражения используя заготовленные упрощения
        TreeSimplifier::simplify(root);

        //Перевести выражение на английский язык по упрощённому дереву
        auto* ctx = new EnglishTranslator::EnglishTranslateContext(data);
        EnglishTranslator translator(ctx);
        QString result = translator.translateExpression(root);

        printf("RESULT: %s\n", qPrintable(result));
        totalResults.append(result);
        delete root;
    }

    //Сохранить результат в файл
    if (!totalResults.isEmpty()) {
        saveResultToFile(totalResults.join("\n\n"), outputPath);
        printf("\nDone! Processed %d lines.\n", totalResults.size());
    }

    return globalError ? 1 : 0;
}
