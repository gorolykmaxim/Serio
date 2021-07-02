#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <ui.h>

class ui_test : public ::testing::Test {
protected:
    ui_data ui_data{};
    std::string actual;
    task task{process_http_response, {"a", 1, true}};
};

static std::string to_string(const nlohmann::json& json) {
    return json.dump();
}

TEST_F(ui_test, should_serialize_view) {
    const auto expected = to_string({{"view", view_id::dialog}});
    ui_data.view = view_id::dialog;
    serialize_ui_data(ui_data, actual);
    EXPECT_EQ(expected, actual);
}

TEST_F(ui_test, should_serialize_animation) {
    const auto expected = to_string({{"view", 0}, {"animation", {{"speed", animation_speed::slow}, {"fade", true}, {"scale", false}}}});
    ui_data.animation = {animation_speed::slow, true, false};
    serialize_ui_data(ui_data, actual);
    EXPECT_EQ(expected, actual);
}

TEST_F(ui_test, should_have_correct_animation_speeds) {
    EXPECT_EQ(0, animation_speed::fast);
    EXPECT_EQ(1, animation_speed::slow);
}

TEST_F(ui_test, should_serialize_dialog) {
    ui_data.dialog = {"title", "description", "cancel", "confirm", task};
    const auto expected = to_string({
        {"view", 0},
        {"dialog", {
            {"title", ui_data.dialog->title},
            {"description", ui_data.dialog->description},
            {"cancelText", ui_data.dialog->cancel_text},
            {"confirmText", ui_data.dialog->confirm_text},
            {"confirmEvent", {{"event", task.type}, {"args", task.args}}}}
        }
    });
    serialize_ui_data(ui_data, actual);
    EXPECT_EQ(expected, actual);
}

TEST_F(ui_test, should_serialize_dialog_without_confirm_event) {
    ui_data.dialog = {"title", "description", "cancel", "confirm"};
    const auto expected = to_string({
        {"view", 0},
        {"dialog", {
            {"title", ui_data.dialog->title},
            {"description", ui_data.dialog->description},
            {"cancelText", ui_data.dialog->cancel_text},
            {"confirmText", ui_data.dialog->confirm_text}}
        }
    });
    serialize_ui_data(ui_data, actual);
    EXPECT_EQ(expected, actual);
}

TEST_F(ui_test, should_serialize_back_event) {
    ui_data.back_event = task;
    const auto expected = to_string({
        {"view", 0},
        {"backEvent", {{"event", task.type}, {"args", task.args},}},
    });
    serialize_ui_data(ui_data, actual);
    EXPECT_EQ(expected, actual);
}

TEST_F(ui_test, should_serialize_edit_text) {
    ui_data.edit_text = {"label", "value", task, task};
    const auto expected = to_string({
        {"view", 0},
        {"editText", {
            {"label", ui_data.edit_text->label},
            {"value", ui_data.edit_text->value},
            {"valueChangedEvent", {{"event", task.type}, {"args", task.args}}},
            {"saveValueEvent", {{"event", task.type}, {"args", task.args}}},
        }},
    });
    serialize_ui_data(ui_data, actual);
    EXPECT_EQ(expected, actual);
}

TEST_F(ui_test, should_have_correct_view_ids) {
    EXPECT_EQ(0, view_id::title_screen);
    EXPECT_EQ(1, view_id::edit_text_dialog);
    EXPECT_EQ(2, view_id::dialog);
    EXPECT_EQ(3, view_id::loading_screen);
}
