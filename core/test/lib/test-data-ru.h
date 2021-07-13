#ifndef SERIO_TEST_DATA_RU_H
#define SERIO_TEST_DATA_RU_H

#include "test-lib.h"

const expected_render_tasks RUSSIAN = {
        "Русский",
        "ru",
        {
                {"dialog", {
                        {"title", "Ссылка на конфигурацию"},
                        {"description", "Укажите ссылку к файлу, который содержит конфигурации кровлеров, ответственных за кровлинг сериалов."},
                        {"cancelText", ""},
                        {"confirmText", "Сохранить"},
                }},
                {"editText", {
                        {"label", "Ссылка на конфигурацию кровлера"},
                        {"value", ""},
                }},
                {"viewId", 1},
        },
        {
                {"loading", {
                        {"text", "Загружаю конфигурацию кровлера..."},
                }},
                {"viewId", 3},
        },
        {
                {"animation", {
                        {"fade", true},
                        {"scale", false},
                        {"speed", 1}
                }},
                {"viewId", 0},
        },
        {
                {"dialog", {
                        {"cancelText", "Изменить ссылку"},
                        {"confirmText", ""},
                        {"description", "Не удалось загрузить '': Failed to send request"},
                        {"title", "Не удалось загрузить конфигурацию кровлера"},
                }},
                {"viewId", 2},
        },
        {
                {"dialog", {
                        {"cancelText", "Изменить ссылку"},
                        {"confirmText", ""},
                        {"description", "Не удалось загрузить 'https://wrong-crawler-config-url.com': Internal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal se..."},
                        {"title", "Не удалось загрузить конфигурацию кровлера"},
                }},
                {"viewId", 2},
        },
        {
                {"dialog", {
                        {"cancelText", "Изменить ссылку"},
                        {"confirmText", ""},
                        {"description", "Не удалось разобрать Internal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal se... Причина: [json.exception.parse_error.101] parse error at 1: syntax error - invalid literal; last read: 'I'"},
                        {"title", "Не удалось разобрать конфигурацию кровлера"},
                }},
                {"viewId", 2},
        },
};

#endif //SERIO_TEST_DATA_RU_H
