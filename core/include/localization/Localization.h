#ifndef SERIO_LOCALIZATION_H
#define SERIO_LOCALIZATION_H

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
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

    Localization();
    explicit Localization(const std::vector<Translation>& translations);
    void setCurrentLanguage(const std::string& language);
    [[nodiscard]] std::string getCurrentLanguage() const;
    [[nodiscard]] std::vector<std::string> getLanguages() const;
    [[nodiscard]] std::string getText(TextId id) const;
    [[nodiscard]] std::string getText(TextId id, const std::string& parameter) const;
private:
    std::map<std::string, std::unordered_map<TextId, std::vector<TranslationRule>>> languageToTextIdToTranslationRules;
    std::string currentLanguage;

    [[nodiscard]] std::string getTextForParameter(TextId id, const std::string& parameter) const;
    void applyParameterToText(std::string& text, const std::string& parameter, TextId id) const;
    [[nodiscard]] bool ruleAppliesToParameter(const std::string& rule, const std::string& parameter) const;
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
