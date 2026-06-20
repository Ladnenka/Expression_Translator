#pragma once
#include "../AbstractTranslator.h"

AbstractTranslator::TranslateContext::LoadedData makeData(
    const QList<Function>& funcs = {},
    const QList<Variable>& vars = {});
