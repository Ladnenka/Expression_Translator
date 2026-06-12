#ifndef ERROR_H
#define ERROR_H
#include <QString>

class Error {
public:
    enum ErrorType {
        UnknownFunction,
        NotEnoughOperands,
        ExtraOperands,
        UnsupportedOperation,
        InvalidArgumentCount,
        FileNotExist,
        FileNotRead,
        JsonParseError,
        MissingRequiredField,
        UnsupportedType,
        EmptyVariableName,
        InvalidCharacters,
        DuplicateVariable,
        DuplicateFunction,
        NoMatchingOverload,
        AmbiguousOverload,
        ExpressionTooLong,
        TooManyOperations,
        EmptyInputFile,
        MissingArguments,
        TooManyExpressions,
        SideEffectConflict
    };

    ErrorType type;
    QString id;
    QString dataTypeStr;
    QString errorFilePath;
    int errorColumn;
    int errorLine;

    Error(ErrorType t, const QString& identifier = "", const QString& dataType = "",
          const QString& filePath = "", int column = -1, int line = -1);

    QString generateErrorMessage() const;
};

#endif // ERROR_H
