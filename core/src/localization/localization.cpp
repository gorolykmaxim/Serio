#include "localization.h"
#include "english.h"
#include "russian.h"

void init_localization(std::vector<const language *> &languages, language const** current_language) {
    languages = {&ENGLISH, &RUSSIAN};
    *current_language = &RUSSIAN;
}

std::string get_text(const std::vector<translation> &translations, text_id text_id) {
    const auto trans = std::find_if(translations.cbegin(), translations.cend(),
                                    [text_id] (const translation& t) { return t.text_id == text_id && t.parameterized_text_ending.empty(); });
    return trans->text;
}

std::string get_text(const std::vector<translation> &translations, text_id text_id, const std::string &parameter) {
    const auto trans = std::find_if(translations.cbegin(), translations.cend(),
                                    [text_id, &parameter] (const translation& t) { return t.text_id == text_id && std::equal(t.parameterized_text_ending.rbegin(), t.parameterized_text_ending.rend(), parameter.rbegin()); });
    auto result = trans->text;
    const auto pos = result.find("{}");
    result.replace(pos, 2, parameter);
    return result;
}
