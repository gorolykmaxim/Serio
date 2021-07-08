#ifndef SERIO_RUSSIAN_H
#define SERIO_RUSSIAN_H

#include <localization.h>

const language RUSSIAN = {
        "Русский",
        {
                {edit_crawler_config_title, "Ссылка на конфигурацию"},
                {edit_crawler_config_description, "Укажите ссылку к файлу, который содержит конфигурации кровлеров, ответственных за кровлинг сериалов."},
                {edit_crawler_config_save, "Сохранить"},
                {edit_crawler_config_label, "Ссылка на конфигурацию кровлера"},
                {downloading_crawler_config, "Загружаю конфигурацию кровлера..."},
                {failed_to_download_crawler_config_title, "Не удалось загрузить конфигурацию кровлера"},
                {failed_to_download_crawler_config_description, "Не удалось загрузить '{}': "},
                {failed_to_parse_crawler_config_title, "Не удалось разобрать конфигурацию кровлера"},
                {failed_to_parse_crawler_config_description, "Не удалось разобрать {} Причина: "},
                {failed_to_get_crawler_config_change_url, "Изменить ссылку"},
        }
};

#endif //SERIO_RUSSIAN_H
