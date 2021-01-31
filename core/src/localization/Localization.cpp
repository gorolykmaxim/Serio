#include "localization/Localization.h"
#include "English.h"
#include "Russian.h"
#include <utility>

namespace serio {
Localization::Localization() : Localization({ENGLISH, RUSSIAN}) {}

Localization::Localization(std::vector<Language> languages) : languages(std::move(languages)) {
    currentLanguage = this->languages[0];
}

void Localization::setCurrentLanguage(const std::string &language) {
    const auto it = std::find_if(languages.cbegin(), languages.cend(),
                                 [&language] (const auto& l) { return language == l.name; });
    if (it == languages.cend()) {
        throw UnsupportedLanguageError(language);
    }
    currentLanguage = *it;
}

std::string Localization::getCurrentLanguage() const {
    return currentLanguage.name;
}

std::vector<std::string> Localization::getLanguages() const {
    std::vector<std::string> languageNames;
    languageNames.reserve(languages.size());
    for (const auto& language: languages) {
        languageNames.push_back(language.name);
    }
    return languageNames;
}

std::string Localization::getText(TextId id) const {
    try {
        return currentLanguage.idToText.at(id);
    } catch (std::out_of_range& e) {
        throw MissingLocalizationError(currentLanguage.name, id);
    }
}

std::string Localization::getText(TextId id, const std::string &parameter1) const {
    auto text = getText(id);
    const auto pos = text.find(PLACEHOLDER);
    if (pos == std::string::npos) {
        throw NotFormattableTextError(currentLanguage.name, id);
    }
    text.replace(pos, PLACEHOLDER.size(), parameter1);
    return text;
}

UnsupportedLanguageError::UnsupportedLanguageError(const std::string &language)
    : logic_error("Can't set current language to an unsupported language: " + language) {}

MissingLocalizationError::MissingLocalizationError(const std::string &language, TextId id)
    : std::logic_error("No localization found for text with ID: " + std::to_string(id) + " in language: " + language) {}

NotFormattableTextError::NotFormattableTextError(const std::string &language, TextId id)
    : std::invalid_argument("Text with ID: " + std::to_string(id) + " of language: " + language + " does not have "
                            "'" + Localization::PLACEHOLDER + "' in it and thus does not support formatting") {}
}