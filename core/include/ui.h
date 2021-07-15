#ifndef SERIO_UI_H
#define SERIO_UI_H

#include <string>
#include "task.h"

enum view_container_id {
    body
};

enum view_id {
    title_screen_view,
    edit_text_dialog_view,
    dialog_view,
    loading_screen_view
};

enum animation_speed {
    fast, slow
};

struct animation {
    view_container_id id;
    animation_speed speed;
    bool fade;
    bool scale;
};

struct dialog {
    std::string title;
    std::string description;
    std::string cancel_text;
    std::string confirm_text;
    std::optional<task> confirm_task;
};

struct edit_text {
    std::string label;
    std::string value;
    task value_changed_task;
    task save_value_task;
};

struct loading {
    std::string text;
};

struct ui_data {
    view_id view_id;
    std::optional<dialog> dialog;
    std::optional<edit_text> edit_text;
    std::optional<loading> loading;
    std::vector<animation> animations;
    std::optional<task> back_task;
    bool rendered = false;
};

void render_ui(ui_data& ui_data, queue<std::string>& render_task_queue);

#endif //SERIO_UI_H
