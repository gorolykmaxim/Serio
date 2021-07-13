#ifndef SERIO_TEST_LIB_H
#define SERIO_TEST_LIB_H

#include <string>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <core.h>

const auto DB_PATH = ":memory:";

struct expected_render_tasks {
    std::string language;
    std::string test_name;
    nlohmann::json edit_crawler_config_url;
    nlohmann::json downloading_crawler_config;
    nlohmann::json title_screen;
    nlohmann::json error_wrong_config_url;
    nlohmann::json error_failed_config_request;
    nlohmann::json error_invalid_config_response;
};

std::string create_long_string(const std::string& word, size_t count);
std::string get_test_name(const testing::TestParamInfo<expected_render_tasks>& info);
void execute_all_core_tasks(core& core);
void send_task(core& core, nlohmann::json task, std::vector<nlohmann::json> args = {});
nlohmann::json get_render_task(core& core);
void expect_equal(const nlohmann::json& e, const nlohmann::json& a);
void pre_init_core_with_params(core& core, const std::string& config_url);

#endif //SERIO_TEST_LIB_H
