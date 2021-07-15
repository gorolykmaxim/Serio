#include "lib/serio-test.h"


TEST_F(functional_test, open_first_time_and_exit) {
    init_core(core, DB_PATH);
    execute_all_core_tasks(core);
    const auto render_task = *core.render_task_queue.try_dequeue();
    EXPECT_FALSE(enqueue_back_task_from_render_task(render_task, core.task_queue));
}

TEST_F(functional_test, open_first_time_enter_all_wrong_urls_then_enter_right_url) {
    init_core(core, DB_PATH);
    execute_all_core_tasks(core);
    // Press confirm while URL is empty
    auto render_task = get_render_task(core);
    expect_equal(ENGLISH.edit_crawler_config_url, render_task);
    send_task(core, render_task["dialog"]["confirmTask"]);
    expect_equal(ENGLISH.downloading_crawler_config, get_render_task(core));
    render_task = get_render_task(core);
    expect_equal(ENGLISH.error_wrong_config_url, render_task);
    send_task(core, render_task["backTask"]);
    // Enter URL to unavailable HTTP resource and press keyboard enter
    render_task = get_render_task(core);
    expect_equal(ENGLISH.edit_crawler_config_url, render_task);
    send_task(core, render_task["editText"]["valueChangedTask"], {WRONG_CONFIG_URL});
    send_task(core, render_task["editText"]["saveValueTask"]);
    expect_equal(ENGLISH.downloading_crawler_config, get_render_task(core));
    render_task = get_render_task(core);
    expect_equal(ENGLISH.error_failed_config_request, render_task);
    enqueue_back_task_from_render_task(render_task.dump(), core.task_queue);
    execute_all_core_tasks(core);
    // Enter URL to HTTP resource with invalid JSON
    render_task = get_render_task(core);
    auto expected_edit_dialog = ENGLISH.edit_crawler_config_url;
    expected_edit_dialog["editText"]["value"] = WRONG_CONFIG_URL;
    expect_equal(expected_edit_dialog, render_task);
    send_task(core, render_task["editText"]["valueChangedTask"], {BROKEN_CONFIG_URL});
    send_task(core, render_task["editText"]["saveValueTask"]);
    expect_equal(ENGLISH.downloading_crawler_config, get_render_task(core));
    render_task = get_render_task(core);
    expect_equal(ENGLISH.error_invalid_config_response, render_task);
    enqueue_back_task_from_render_task(render_task.dump(), core.task_queue);
    execute_all_core_tasks(core);
    // Enter correct config URL
    render_task = get_render_task(core);
    expected_edit_dialog["editText"]["value"] = BROKEN_CONFIG_URL;
    expect_equal(expected_edit_dialog, render_task);
    send_task(core, render_task["editText"]["valueChangedTask"], {CONFIG_URL});
    send_task(core, render_task["editText"]["saveValueTask"]);
    expect_equal(ENGLISH.downloading_crawler_config, get_render_task(core));
    render_task = get_render_task(core);
    expect_equal(ENGLISH.title_screen, render_task);
    // Press "Back" button while being on title screen.
    EXPECT_FALSE(enqueue_back_task_from_render_task(render_task.dump(), core.task_queue));
}

// TODO: make following tests locale-parameterized (once config view is finished)
TEST_F(functional_test, should_display_title_screen_and_successfully_download_predefined_config) {
    pre_init_core_with_params(core, CONFIG_URL);
    init_core(core, DB_PATH);
    execute_all_core_tasks(core);
    expect_equal(ENGLISH.title_screen, get_render_task(core));
}

TEST_F(functional_test,
       should_get_config_from_cache_and_display_title_screen_if_config_was_previously_specified_even_if_internet_goes_down) {
    pre_init_core_with_params(core, CONFIG_URL);
    http_mock->mock_no_internet();
    init_core(core, DB_PATH);
    execute_all_core_tasks(core);
    expect_equal(ENGLISH.title_screen, get_render_task(core));
}

TEST_F(functional_test, should_exit_app_on_pressing_back_while_being_on_title_screen) {
    pre_init_core_with_params(core, CONFIG_URL);
    init_core(core, DB_PATH);
    execute_all_core_tasks(core);
    const auto render_task = *core.render_task_queue.try_dequeue();
    EXPECT_FALSE(enqueue_back_task_from_render_task(render_task, core.task_queue));
}
// TODO: write additional tests for case when we start with config URL specified and request fails for all the possible
// reasons (once config view is finished and test framework can init core with cache cleared).