#include "TestUtils.h"

AbstractTranslator::TranslateContext::LoadedData makeData(
    const QList<Function>& funcs,
    const QList<Variable>& vars)
{
    AbstractTranslator::TranslateContext::LoadedData data;
    data.functions = funcs;
    data.variables = vars;
    for (const Function& f : funcs) {
        data.functionNames << f.name;
        data.functionArgCount[f.name] = f.paramNames.size();
    }
    return data;
}
