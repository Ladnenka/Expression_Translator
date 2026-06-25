#include "../Error.h"

Error::Error(ErrorType t, const QString& identifier, const QString& dataType,
             const QString& filePath, int column, int line)
    : type(t), id(identifier), dataTypeStr(dataType), errorFilePath(filePath),
    errorColumn(column), errorLine(line) {}

QString Error::generateErrorMessage() const {
    switch (type) {
    case FileNotExist:
    case FileNotRead:
        return "Input file " + errorFilePath + " is incorrect. The file may not exist or cannot be accessed.";

    case EmptyInputFile:
        return "Input file " + errorFilePath + " contains no data.";

    case JsonParseError:
        return "Invalid JSON format in input file " + errorFilePath + ".";

    case MissingRequiredField:
        return "Required field " + id + " missing in JSON file " + errorFilePath + ".";

    case UnsupportedType:
        return "Unsupported data type " + dataTypeStr + " for variable " + id + " in file " + errorFilePath + ".";

    case DuplicateVariable:
        return "Duplicate variable name " + id + " detected in file " + errorFilePath + ".";

    case DuplicateFunction:
        return "Duplicate function name " + id + " with the same parameter set detected in file " + errorFilePath + ".";

    case EmptyVariableName:
        return "Variable/function name cannot be empty in file " + errorFilePath + ".";

    case InvalidCharacters:
        return "Variable/function " + id + " contains invalid characters in field " + dataTypeStr + " in file " + errorFilePath + ".";

    case MissingArguments:
        return errorColumn == 0
                   ? "Missing required program arguments. Expected: <input.txt> <output.txt>"
                   : "Missing required program argument. Expected: <output.txt>";

    case UnsupportedOperation:
        return "Unsupported operation " + id + " at position " + QString::number(errorLine) + ".";

    case ExpressionTooLong:
        return "Expression length exceeds 1000 characters in file " + errorFilePath + ".";

    case TooManyOperations:
        return "Expression contains more than 100 operations.";

    case InvalidArgumentCount:
        return "Function " + id + " called with incorrect number of arguments at position " +
               QString::number(errorLine) + ". Expected " + dataTypeStr + ".";

    case UnknownFunction:
        return "Use of unknown variable or function " + id + " at position " + QString::number(errorLine) + ".";

    case AmbiguousOverload:
        return "Ambiguous call to overloaded function " + id + " at position " +
               QString::number(errorLine) + ". Multiple candidates match.";

    case NoMatchingOverload:
        return "No matching overload for function " + id + " at position " +
               QString::number(errorLine) + " for the provided arguments.";

    case ExtraOperands:
        return "Expression contains extra operands. Stack contains " + dataTypeStr + " values after evaluation.";

    case NotEnoughOperands:
        return "Expression lacks operations on operands to complete successfully. " +
               dataTypeStr + " operand/s left unprocessed.";

    case SideEffectConflict:
        return "Variable " + id + " is modified more than once in expression at position " +
               QString::number(errorLine) + ". Operations with side effect are not supported.";

    case TooManyExpressions:
        return "Too many expressions! The input file contains more than 10 expressions. Maximum allowed is 10.";

    default:
        return "Unknown error.";
    }
}
