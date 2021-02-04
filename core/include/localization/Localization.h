#ifndef SERIO_LOCALIZATION_H
#define SERIO_LOCALIZATION_H

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <config/Config.h>
#include "TextId.h"

namespace serio {
struct Translation {
    std::string language;
    TextId textId;
    std::string text;
    std::string parameterRule;
};

struct TranslationRule {
    std::string text;
    std::string parameterRule;
};

class Localization {
public:
    inline static const std::string PLACEHOLDER = "{}";
    inline static const std::string CURRENT_LANGUAGE_CONFIG_PROPERTY = "current-language";

    explicit Localization(Config& config);
    Localization(const std::vector<Translation>& translations, Config& config);
    void setCurrentLanguage(const std::string& language);
    [[nodiscard]] std::string getCurrentLanguage() const;
    [[nodiscard]] std::vector<std::string> getLanguages() const;
    [[nodiscard]] std::string getText(TextId id) const;
    [[nodiscard]] std::string getText(TextId id, const std::string& parameter) const;
private:
    Config& config;
    std::map<std::string, std::unordered_map<TextId, std::vector<TranslationRule>>> languageToTextIdToTranslationRules;
    std::unordered_map<TextId, std::vector<TranslationRule>>* textIdToTranslationRules;

    [[nodiscard]] std::string getTextForParameter(TextId id, const std::string& parameter) const;
    void applyParameterToText(std::string& text, const std::string& parameter, TextId id) const;
    [[nodiscard]] bool ruleAppliesToParameter(const std::string& rule, const std::string& parameter) const;
    void loadTranslations(const std::vector<Translation>& translations);
    void updateCurrentLanguageIfNecessary(const std::vector<Translation>& translations);
};

class NoTranslationsProvidedError: public std::logic_error {
public:
    NoTranslationsProvidedError();
};

class UnsupportedLanguageError: public std::logic_error {
public:
    explicit UnsupportedLanguageError(const std::string &language);
};

class MissingLocalizationError: public std::logic_error {
public:
    MissingLocalizationError(const std::string& language, TextId id);
};

class NotFormattableTextError: public std::invalid_argument {
public:
    NotFormattableTextError(const std::string& language, TextId id);
};
}

#endif //SERIO_LOCALIZATION_H
