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
    const auto expected = to_string({{"viewId", view_id::dialog}});
    ui_data.view_id = view_id::dialog;
    serialize_ui_data(ui_data, actual);
    EXPECT_EQ(expected, actual);
}

TEST_F(ui_test, should_serialize_animation) {
    const auto expected = to_string({{"viewId", 0}, {"animation", {{"speed", animation_speed::slow}, {"fade", true}, {"scale", false}}}});
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
        {"viewId", 0},
        {"dialog", {
            {"title", ui_data.dialog->title},
            {"description", ui_data.dialog->description},
            {"cancelText", ui_data.dialog->cancel_text},
            {"confirmText", ui_data.dialog->confirm_text},
            {"confirmTask", {{"taskId", task.id}, {"args", task.args}}}}
        }
    });
    serialize_ui_data(ui_data, actual);
    EXPECT_EQ(expected, actual);
}

TEST_F(ui_test, should_serialize_dialog_without_confirm_task) {
    ui_data.dialog = {"title", "description", "cancel", "confirm"};
    const auto expected = to_string({
        {"viewId", 0},
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

TEST_F(ui_test, should_serialize_back_task) {
    ui_data.back_task = task;
    const auto expected = to_string({
        {"viewId", 0},
        {"backTask", {{"taskId", task.id}, {"args", task.args},}},
    });
    serialize_ui_data(ui_data, actual);
    EXPECT_EQ(expected, actual);
}

TEST_F(ui_test, should_serialize_edit_text) {
    ui_data.edit_text = {"label", "value", task, task};
    const auto expected = to_string({
        {"viewId", 0},
        {"editText", {
            {"label", ui_data.edit_text->label},
            {"value", ui_data.edit_text->value},
            {"valueChangedTask", {{"taskId", task.id}, {"args", task.args}}},
            {"saveValueTask", {{"taskId", task.id}, {"args", task.args}}},
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

TEST_F(ui_test, should_display_fatal_error_view) {
    std::string actual;
    const std::string error_msg = "Really helpful and informative error message";
    const nlohmann::json expected = {
        {"viewId", view_id::dialog},
        {"dialog", {
            {"title", "Fatal error :("},
            {"description", "Tell developer and restart the application: " + error_msg},
            {"cancelText", ""},
            {"confirmText", ""}}
        },
    };
    display_fatal_error(error_msg, [&actual] (const std::string& view) { actual = view; });
    EXPECT_EQ(expected.dump(), actual);
}