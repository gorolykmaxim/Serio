#ifndef SERIO_ENGLISH_H
#define SERIO_ENGLISH_H

#include <localization.h>

const language ENGLISH = {
    "English",
    {
        {edit_crawler_config_title, "Crawler config URL"},
        {edit_crawler_config_description, "Specify URL to the file, that contains configuration of all the crawlers, responsible for crawling tv shows."},
        {edit_crawler_config_save, "Save"},
        {edit_crawler_config_label, "Crawler config URL"},
        {downloading_crawler_config, "Downloading crawler config..."},
        {failed_to_download_crawler_config_title, "Failed to download crawler config"},
        {failed_to_download_crawler_config_description, "Failed to download '{}': "},
        {failed_to_parse_crawler_config_title, "Failed to parse crawler config"},
        {failed_to_parse_crawler_config_description, "Failed to parse {} Reason: "},
        {failed_to_get_crawler_config_change_url, "Change URL"},
    }
};

#endif //SERIO_ENGLISH_H
