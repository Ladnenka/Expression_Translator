#include "AbstractTranslator.h"

AbstractTranslator::AbstractTranslator(TranslateContext* ctx) : context(ctx) {
    context->translator = this;
}

AbstractTranslator::TranslateContext::LoadedData::LoadedData() {}
