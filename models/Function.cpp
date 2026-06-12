#include "../Function.h"

Function::Function(const QString& n, const QStringList& pn, const QString& td)
    : name(n), paramNames(pn), templateDesc(td) {}
