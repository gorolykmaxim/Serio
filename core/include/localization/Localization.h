#ifndef SERIO_LOCALIZATION_H
#define SERIO_LOCALIZATION_H

#include <string>
#include <vector>
#include <unordered_map>
#include "TextId.h"

namespace serio {
struct Language {
    std::string name;
    std::unordered_map<TextId, std::string> idToText;
};

class Localization {
public:
    inline static const std::string PLACEHOLDER = "{}";

    Localization();
    explicit Localization(std::vector<Language> languages);
    void setCurrentLanguage(const std::string& language);
    [[nodiscard]] std::string getCurrentLanguage() const;
    [[nodiscard]] std::vector<std::string> getLanguages() const;
    [[nodiscard]] std::string getText(TextId id) const;
    [[nodiscard]] std::string getText(TextId id, const std::string& parameter1) const;
private:
    std::vector<Language> languages;
    Language currentLanguage;
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
