#include "localization/Localization.h"
#include "English.h"
#include "Russian.h"
#include <utility>
#include <algorithm>

namespace serio {
static std::vector<Translation> flatten(const std::vector<std::vector<Translation>>& languages) {
    std::vector<Translation> translations;
    for (const auto& language: languages) {
        translations.insert(translations.cend(), language.cbegin(), language.cend());
    }
    return translations;
}

Localization::Localization() : Localization(flatten({ENGLISH, RUSSIAN})) {}

Localization::Localization(const std::vector<Translation>& translations) {
    if (translations.empty()) {
        return;
    }
    currentLanguage = translations[0].language;
    for (const auto& translation: translations) {
        languageToTextIdToTranslationRules[translation.language][translation.textId].push_back({translation.text, translation.parameterRule});
    }
}

void Localization::setCurrentLanguage(const std::string &language) {
    const auto it = languageToTextIdToTranslationRules.find(language);
    if (it == languageToTextIdToTranslationRules.cend()) {
        throw UnsupportedLanguageError(language);
    }
    currentLanguage = language;
}

std::string Localization::getCurrentLanguage() const {
    return currentLanguage;
}

std::vector<std::string> Localization::getLanguages() const {
    std::vector<std::string> languageNames;
    languageNames.reserve(languageToTextIdToTranslationRules.size());
    for (const auto& languageToText: languageToTextIdToTranslationRules) {
        languageNames.push_back(languageToText.first);
    }
    return languageNames;
}

std::string Localization::getText(TextId id) const {
    try {
        return languageToTextIdToTranslationRules.at(currentLanguage).at(id).back().text;
    } catch (std::out_of_range& e) {
        throw MissingLocalizationError(currentLanguage, id);
    }
}

std::string Localization::getText(TextId id, const std::string &parameter) const {
    try {
        auto text = getTextForParameter(id, parameter);
        applyParameterToText(text, parameter, id);
        return text;
    } catch (std::out_of_range& e) {
        throw MissingLocalizationError(currentLanguage, id);
    }
}

std::string Localization::getTextForParameter(TextId id, const std::string &parameter) const {
    const auto rules = languageToTextIdToTranslationRules.at(currentLanguage).at(id);
    const auto it = std::find_if(rules.cbegin(), rules.cend(),
                                 [this, &parameter] (const TranslationRule& r) { return ruleAppliesToParameter(r.parameterRule, parameter); });
    if (it == rules.cend()) {
        throw std::out_of_range("");
    }
    return it->text;
}

void Localization::applyParameterToText(std::string &text, const std::string &parameter, TextId id) const {
    const auto pos = text.find(PLACEHOLDER);
    if (pos == std::string::npos) {
        throw NotFormattableTextError(currentLanguage, id);
    }
    text.replace(pos, PLACEHOLDER.size(), parameter);
}

bool Localization::ruleAppliesToParameter(const std::string &rule, const std::string &parameter) const {
    return std::equal(rule.rbegin(), rule.rend(), parameter.rbegin());
}

UnsupportedLanguageError::UnsupportedLanguageError(const std::string &language)
    : logic_error("Can't set current language to an unsupported language: " + language) {}

MissingLocalizationError::MissingLocalizationError(const std::string &language, TextId id)
    : std::logic_error("No localization found for text with ID: " + std::to_string(id) + " in language: " + language) {}

NotFormattableTextError::NotFormattableTextError(const std::string &language, TextId id)
    : std::invalid_argument("Text with ID: " + std::to_string(id) + " of language: " + language + " does not have "
                            "'" + Localization::PLACEHOLDER + "' in it and thus does not support formatting") {}
}