#include <nlohmann/json.hpp>
#include "ui.h"

static void serialize_task(const std::optional<task>& task, nlohmann::json& task_container, const std::string& task_name) {
    if (task) {
        nlohmann::json event;
        event["event"] = task->type;
        event["args"] = task->args;
        task_container[task_name] = event;
    }
}

void serialize_ui_data(const ui_data &ui_data, std::string& result) {
    nlohmann::json json_res;
    json_res["view"] = ui_data.view;
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
        serialize_task(ui_data.dialog->confirm_event, dialog, "confirmEvent");
        json_res["dialog"] = dialog;
    }
    if (ui_data.edit_text) {
        nlohmann::json edit_text;
        edit_text["label"] = ui_data.edit_text->label;
        edit_text["value"] = ui_data.edit_text->value;
        serialize_task(ui_data.edit_text->value_changed_event, edit_text, "valueChangedEvent");
        serialize_task(ui_data.edit_text->save_value_event, edit_text, "saveValueEvent");
        json_res["editText"] = edit_text;
    }
    serialize_task(ui_data.back_event, json_res, "backEvent");
    result = json_res.dump();
}
