#include "../Error.h"

Error::Error(ErrorType t, const QString& identifier, const QString& dataType,
             const QString& filePath, int column, int line, const QString& extra)
    : type(t), id(identifier), dataTypeStr(dataType), extraInfo(extra),
    errorFilePath(filePath), errorColumn(column), errorLine(line) {}

QString Error::generateErrorMessage() const {
    //Сформировать сообщение об ошибке в зависимости от её типа
    switch (type) {

    //Ошибки доступа к файлу
    case FileNotExist:
    case FileNotRead:
        return "Input file " + errorFilePath + " is incorrect. The file may not exist or cannot be accessed.";

    //Ошибки содержимого входного файла
    case EmptyInputFile:
        return "Input file " + errorFilePath + " contains no data.";
    case JsonParseError:
        return "Invalid JSON format in input file " + errorFilePath + ".";

    //Ошибки структуры JSON
    case MissingRequiredField:
        //Если объект имеет имя — включить его в сообщение
        return extraInfo.isEmpty()
                   ? "Required field '" + dataTypeStr + "' missing in object #" + id + " in JSON file " + errorFilePath + "."
                   : "Required field '" + dataTypeStr + "' missing in object '" + extraInfo + "' (#" + id + ") in JSON file " + errorFilePath + ".";
    case UnsupportedType:
        return "Unsupported data type '" + dataTypeStr + "' in field 'type' of variable '" + extraInfo + "' in object #" + id + " in JSON file " + errorFilePath + ".";
    case DuplicateVariable:
        return "Duplicate variable name '" + dataTypeStr + "' detected in object #" + id + " in JSON file " + errorFilePath + ".";
    case DuplicateFunction:
        return "Duplicate function name '" + dataTypeStr + "' with the same parameter set detected in object #" + id + " in JSON file " + errorFilePath + ".";
    case EmptyVariableName:
        return "Variable/function name cannot be empty in object #" + id + " in JSON file " + errorFilePath + ".";
    case InvalidCharacters:
        return "Invalid characters '" + extraInfo + "' in field '" + dataTypeStr + "' of object #" + id + " in JSON file " + errorFilePath + ".";

    //Ошибки аргументов командной строки
    case MissingArguments:
        //Если не передан ни один аргумент — сообщить об обоих, иначе только о выходном файле
        return errorColumn == 0
                   ? "Missing required program arguments. Expected: <input.txt> <output.txt>"
                   : "Missing required program argument. Expected: <output.txt>";

    //Ошибки выражения
    case UnsupportedOperation:
        return "Unsupported operation " + id + " at position " + QString::number(errorLine) + ".";
    case ExpressionTooLong:
        return "Expression length exceeds 1000 characters in file " + errorFilePath + ".";
    case TooManyOperations:
        return "Expression contains more than 100 operations.";
    case TooManyExpressions:
        return "Too many expressions! The input file contains more than 10 expressions. Maximum allowed is 10.";

    //Ошибки построения дерева
    case InvalidArgumentCount:
        return "Function " + id + " called with incorrect number of arguments at position " +
               QString::number(errorLine) + ". Expected " + dataTypeStr + ".";
    case UnknownFunction:
        return "Use of unknown variable or function " + id + " at position " + QString::number(errorLine) + ".";
    case ExtraOperands:
        return "Expression contains extra operands. Stack contains " + dataTypeStr + " values after evaluation.";
    case NotEnoughOperands:
        return "Expression lacks operations on operands to complete successfully. " +
               dataTypeStr + " operand/s left unprocessed.";
    case SideEffectConflict:
        return "Variable " + id + " is modified more than once in expression at position " +
               QString::number(errorLine) + ". Operations with side effect are not supported.";

    default:
        return "Unknown error.";
    }
}
