#include "../Variable.h"

Variable::Variable() {}

Variable::Variable(const QString& n, const QString& d, const QString& t)
    : name(n), description(d), type(t) {}
