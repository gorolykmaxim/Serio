#ifndef SERIO_TEST_DATA_EN_H
#define SERIO_TEST_DATA_EN_H

#include "test-lib.h"

const expected_render_tasks ENGLISH = {
        "English",
        "en",
        {
                {"dialog", {
                        {"title", "Crawler config URL"},
                        {"description", "Specify URL to the file, that contains configuration of all the crawlers, responsible for crawling tv shows."},
                        {"cancelText", ""},
                        {"confirmText", "Save"},
                }},
                {"editText", {
                        {"label", "Crawler config URL"},
                        {"value", ""},
                }},
                {"viewId", 1},
        },
        {
                {"loading", {
                        {"text", "Downloading crawler config..."},
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
                        {"cancelText", "Change URL"},
                        {"confirmText", ""},
                        {"description", "Failed to download '': Failed to send request"},
                        {"title", "Failed to download crawler config"},
                }},
                {"viewId", 2},
        },
        {
                {"dialog", {
                        {"cancelText", "Change URL"},
                        {"confirmText", ""},
                        {"description", "Failed to download 'https://wrong-crawler-config-url.com': Internal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal se..."},
                        {"title", "Failed to download crawler config"},
                }},
                {"viewId", 2},
        },
        {
                {"dialog", {
                        {"cancelText", "Change URL"},
                        {"confirmText", ""},
                        {"description", "Failed to parse Internal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal server errorInternal se... Reason: [json.exception.parse_error.101] parse error at 1: syntax error - invalid literal; last read: 'I'"},
                        {"title", "Failed to parse crawler config"},
                }},
                {"viewId", 2},
        },
};

#endif //SERIO_TEST_DATA_EN_H
