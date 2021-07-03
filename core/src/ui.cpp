#include <nlohmann/json.hpp>
#include "ui.h"

static void serialize_task(const std::optional<task>& task, nlohmann::json& task_container, const std::string& task_name) {
    if (task) {
        nlohmann::json json_task;
        json_task["taskId"] = task->id;
        json_task["args"] = task->args;
        task_container[task_name] = json_task;
    }
}

void serialize_ui_data(const ui_data &ui_data, std::string& result) {
    nlohmann::json json_res;
    json_res["viewId"] = ui_data.view_id;
    if (ui_data.animation) {
        nlohmann::json anim;
        anim["speed"] = ui_data.animation->speed;
        anim["fade"] = ui_data.animation->fade;
        anim["scale"] = ui_data.animation->scale;
        json_res["animation"] = anim;
    }
    if (ui_data.dialog) {
        nlohmann::json dialog;
        dialog["title"] = ui_data.dialog->title;
        dialog["description"] = ui_data.dialog->description;
        dialog["cancelText"] = ui_data.dialog->cancel_text;
        dialog["confirmText"] = ui_data.dialog->confirm_text;
        serialize_task(ui_data.dialog->confirm_task, dialog, "confirmTask");
        json_res["dialog"] = dialog;
    }
    if (ui_data.edit_text) {
        nlohmann::json edit_text;
        edit_text["label"] = ui_data.edit_text->label;
        edit_text["value"] = ui_data.edit_text->value;
        serialize_task(ui_data.edit_text->value_changed_task, edit_text, "valueChangedTask");
        serialize_task(ui_data.edit_text->save_value_task, edit_text, "saveValueTask");
        json_res["editText"] = edit_text;
    }
    serialize_task(ui_data.back_task, json_res, "backTask");
    result = json_res.dump();
}

void display_fatal_error(const std::string &error, const render_view &render_view) {
    ui_data ui_data{view_id::dialog};
    ui_data.dialog = {"Fatal error :(", "Tell developer and restart the application: " + error};
    std::string view;
    serialize_ui_data(ui_data, view);
    render_view(view);
}