#include "EnglishTranslator.h"

EnglishTranslator::EnglishTranslateContext::EnglishTranslateContext(const AbstractTranslator::TranslateContext::LoadedData& data)
    : TranslateContext(data) {}

EnglishTranslator::EnglishTranslator(EnglishTranslateContext* ctx)
    : AbstractTranslator(ctx) {}
