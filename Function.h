#ifndef FUNCTION_H
#define FUNCTION_H
#include <QString>
#include <QStringList>

class Function {
public:
    QString name;
    QStringList paramNames;
    QStringList paramTypes;
    QString templateDesc;

    Function();
    ~Function();
};

#endif // FUNCTION_H
