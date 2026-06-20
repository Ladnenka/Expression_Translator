#include "../AbstractTranslator.h"

AbstractTranslator::TranslateContext::~TranslateContext() {}

AbstractTranslator::AbstractTranslator(TranslateContext* ctx) : context(ctx) {
    context->translator = this;
}

AbstractTranslator::~AbstractTranslator() { delete context; }

AbstractTranslator::TranslateContext::LoadedData::LoadedData() {}
