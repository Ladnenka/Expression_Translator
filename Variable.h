#ifndef VARIABLE_H
#define VARIABLE_H
#include <QString>

class Variable {
public:
    QString name;
    QString description;
    QString type;

    Variable();
    Variable(const QString& n, const QString& d, const QString& t);
};

#endif // VARIABLE_H
