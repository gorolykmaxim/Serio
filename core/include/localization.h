#ifndef SERIO_LOCALIZATION_H
#define SERIO_LOCALIZATION_H

#include <vector>
#include <string>

enum text_id {
    edit_crawler_config_title,
    edit_crawler_config_description,
    edit_crawler_config_save,
    edit_crawler_config_label,
    downloading_crawler_config,
    failed_to_download_crawler_config_title,
    failed_to_download_crawler_config_description,
    failed_to_parse_crawler_config_title,
    failed_to_parse_crawler_config_description,
    failed_to_get_crawler_config_change_url,
};

struct translation {
    text_id text_id;
    std::string text;
    std::string parameterized_text_ending;
};

struct language {
    std::string name;
    std::vector<translation> translations;
};

void init_localization(std::vector<const language*>& languages);
std::string get_text(const std::vector<translation>& translations, text_id text_id);
std::string get_text(const std::vector<translation>& translations, text_id text_id, const std::string& parameter);

#endif //SERIO_LOCALIZATION_H
