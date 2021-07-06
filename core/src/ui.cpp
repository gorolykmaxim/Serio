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

void render_ui(ui_data &ui_data, queue<std::string>& render_task_queue) {
    if (ui_data.rendered) return;
    nlohmann::json render_task;
    render_task["viewId"] = ui_data.view_id;
    if (ui_data.animation) {
        nlohmann::json anim;
        anim["speed"] = ui_data.animation->speed;
        anim["fade"] = ui_data.animation->fade;
        anim["scale"] = ui_data.animation->scale;
        render_task["animation"] = anim;
    }
    if (ui_data.dialog) {
        nlohmann::json dialog;
        dialog["title"] = ui_data.dialog->title;
        dialog["description"] = ui_data.dialog->description;
        dialog["cancelText"] = ui_data.dialog->cancel_text;
        dialog["confirmText"] = ui_data.dialog->confirm_text;
        serialize_task(ui_data.dialog->confirm_task, dialog, "confirmTask");
        render_task["dialog"] = dialog;
    }
    if (ui_data.edit_text) {
        nlohmann::json edit_text;
        edit_text["label"] = ui_data.edit_text->label;
        edit_text["value"] = ui_data.edit_text->value;
        serialize_task(ui_data.edit_text->value_changed_task, edit_text, "valueChangedTask");
        serialize_task(ui_data.edit_text->save_value_task, edit_text, "saveValueTask");
        render_task["editText"] = edit_text;
    }
    if (ui_data.loading) {
        nlohmann::json loading;
        loading["text"] = ui_data.loading->text;
        render_task["loading"] = loading;
    }
    serialize_task(ui_data.back_task, render_task, "backTask");
    render_task_queue.enqueue(render_task.dump());
    ui_data.rendered = true;
}
