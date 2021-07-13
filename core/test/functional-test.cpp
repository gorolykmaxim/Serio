#include "lib/serio-test.h"

TEST_F(functional_test,
       should_ask_user_to_enter_crawler_config_url_save_it_on_enter_and_successfully_download_it_for_the_first_time) {
    init_core(core, DB_PATH);
    execute_all_core_tasks(core);
    const auto render_task = get_render_task(core);
    expect_equal(ENGLISH.edit_crawler_config_url, render_task);
    send_task(core, render_task["editText"]["valueChangedTask"], {CONFIG_URL.substr(0, 5)});
    send_task(core, render_task["editText"]["valueChangedTask"], {CONFIG_URL});
    send_task(core, render_task["editText"]["saveValueTask"]);
    expect_equal(ENGLISH.downloading_crawler_config, get_render_task(core));
    expect_equal(ENGLISH.title_screen, get_render_task(core));
}

TEST_F(functional_test,
       should_ask_user_to_enter_crawler_config_url_save_it_on_button_click_and_successfully_download_it_for_the_first_time) {
    init_core(core, DB_PATH);
    execute_all_core_tasks(core);
    const auto render_task = get_render_task(core);
    expect_equal(ENGLISH.edit_crawler_config_url, render_task);
    send_task(core, render_task["editText"]["valueChangedTask"], {CONFIG_URL.substr(0, 5)});
    send_task(core, render_task["editText"]["valueChangedTask"], {CONFIG_URL});
    send_task(core, render_task["dialog"]["confirmTask"]);
    expect_equal(ENGLISH.downloading_crawler_config, get_render_task(core));
    expect_equal(ENGLISH.title_screen, get_render_task(core));
}

TEST_F(functional_test, should_fail_to_download_config_for_the_first_time_due_to_invalid_url) {
    init_core(core, DB_PATH);
    execute_all_core_tasks(core);
    auto render_task = get_render_task(core);
    send_task(core, render_task["editText"]["valueChangedTask"], {""});
    send_task(core, render_task["editText"]["saveValueTask"]);
    expect_equal(ENGLISH.downloading_crawler_config, get_render_task(core));
    render_task = get_render_task(core);
    expect_equal(ENGLISH.error_wrong_config_url, render_task);
    send_task(core, render_task["backTask"]);
    expect_equal(ENGLISH.edit_crawler_config_url, get_render_task(core));
}


TEST_F(functional_test, should_fail_to_download_config_for_the_first_time_due_to_error_response_code) {
    init_core(core, DB_PATH);
    execute_all_core_tasks(core);
    auto render_task = get_render_task(core);
    send_task(core, render_task["editText"]["valueChangedTask"], {WRONG_CONFIG_URL});
    send_task(core, render_task["editText"]["saveValueTask"]);
    expect_equal(ENGLISH.downloading_crawler_config, get_render_task(core));
    render_task = get_render_task(core);
    expect_equal(ENGLISH.error_failed_config_request, render_task);
    send_task(core, render_task["backTask"]);
    auto expected_edit_crawler_config_url = ENGLISH.edit_crawler_config_url;
    expected_edit_crawler_config_url["editText"]["value"] = WRONG_CONFIG_URL;
    expect_equal(expected_edit_crawler_config_url, get_render_task(core));
}

TEST_F(functional_test, should_fail_to_parse_config_for_the_first_time_due_to_invalid_response_body) {
    init_core(core, DB_PATH);
    execute_all_core_tasks(core);
    auto render_task = get_render_task(core);
    send_task(core, render_task["editText"]["valueChangedTask"], {BROKEN_CONFIG_URL});
    send_task(core, render_task["editText"]["saveValueTask"]);
    expect_equal(ENGLISH.downloading_crawler_config, get_render_task(core));
    render_task = get_render_task(core);
    expect_equal(ENGLISH.error_invalid_config_response, render_task);
    send_task(core, render_task["backTask"]);
    auto expected_edit_crawler_config_url = ENGLISH.edit_crawler_config_url;
    expected_edit_crawler_config_url["editText"]["value"] = BROKEN_CONFIG_URL;
    expect_equal(expected_edit_crawler_config_url, get_render_task(core));
}

TEST_P(localized_functional_test, should_display_title_screen_and_successfully_download_predefined_config) {
    const auto expected = GetParam();
    pre_init_core_with_params(core, expected.language, CONFIG_URL);
    init_core(core, DB_PATH);
    execute_all_core_tasks(core);
    expect_equal(expected.title_screen, get_render_task(core));
}

TEST_P(localized_functional_test,
       should_get_config_from_cache_and_display_title_screen_if_config_was_previously_specified_even_if_internet_goes_down) {
    const auto expected = GetParam();
    pre_init_core_with_params(core, expected.language, CONFIG_URL);
    http_mock->mock_no_internet();
    init_core(core, DB_PATH);
    execute_all_core_tasks(core);
    expect_equal(expected.title_screen, get_render_task(core));
}

INSTANTIATE_TEST_SUITE_P(localized_functional_test_instantiation,
                         localized_functional_test,
                         testing::Values(ENGLISH, RUSSIAN),
                         get_test_name);