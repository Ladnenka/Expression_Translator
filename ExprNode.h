#ifndef EXPRNODE_H
#define EXPRNODE_H
#include <QString>
#include <QVector>

class ExprNode {
public:
    enum ExprType {
        NUMBER,
        VARIABLE,
        FUNCTION,
        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE,
        MODULO,
        POWER,
        UNARY_MINUS,
        LOGICAL_AND,
        LOGICAL_OR,
        LOGICAL_NOT,
        LESS,
        GREATER,
        LESS_EQ,
        GREATER_EQ,
        EQUAL,
        NOT_EQUAL,
        ASSIGN,
        PLUS_ASSIGN,
        MINUS_ASSIGN,
        MULT_ASSIGN,
        DIV_ASSIGN,
        MOD_ASSIGN,
        PRE_INC,
        POST_INC,
        PRE_DEC,
        POST_DEC,
        ADDRESS_OF,
        DEREFERENCE,
        INDEX
    };

    ExprType type;
    QVector<ExprNode*> operands;
    QString value;
    QString varName;
    QString funcName;

    ExprNode(ExprType t);
    ExprNode(ExprType t, const QString& val);
    ExprNode(double num);
    ExprNode(const QString& var);
    ExprNode(ExprType t, const QString& name, const QVector<ExprNode*>& args);
};

#endif // EXPRNODE_H
