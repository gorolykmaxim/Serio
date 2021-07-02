#ifndef SERIO_UI_H
#define SERIO_UI_H

#include <string>
#include "task.h"

typedef std::function<void(const std::string&)> render_view;

enum view_id {
    title_screen, edit_text_dialog, dialog, loading_screen
};

enum animation_speed {
    fast, slow
};

struct animation {
    animation_speed speed;
    bool fade;
    bool scale;
};

struct dialog {
    std::string title;
    std::string description;
    std::string cancel_text;
    std::string confirm_text;
    std::optional<task> confirm_event;
};

struct edit_text {
    std::string label;
    std::string value;
    task value_changed_event;
    task save_value_event;
};

struct ui_data {
    view_id view;
    std::optional<struct dialog> dialog;
    std::optional<edit_text> edit_text;
    std::optional<animation> animation;
    std::optional<task> back_event;
};

void serialize_ui_data(const ui_data& ui_data, std::string& result);
void display_fatal_error(const std::string& error, const render_view& render_view);

#endif //SERIO_UI_H
