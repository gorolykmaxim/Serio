#include "test-lib.h"
#include <sstream>
#include <utility>

std::string create_long_string(const std::string& word, size_t count) {
    std::stringstream res;
    for (auto i = 0; i < count; i++) {
        res << word;
    }
    return res.str();
}

std::string get_test_name(const testing::TestParamInfo<expected_render_tasks>& info) {
    return info.param.test_name;
}

void execute_all_core_tasks(core& core) {
    while (!core.task_queue.empty()) {
        execute_core_task(core);
    }
}

void send_task(core& core, nlohmann::json task, std::vector<nlohmann::json> args) {
    task["args"] = std::move(args);
    enqueue_core_task(task.dump(), core.task_queue);
    execute_all_core_tasks(core);
}

nlohmann::json get_render_task(core& core) {
    const auto render_task = core.render_task_queue.try_dequeue();
    EXPECT_TRUE(render_task) << "No render task available";
    return nlohmann::json::parse(*render_task);
}

void expect_equal(const nlohmann::json& e, const nlohmann::json& a) {
    if (e.is_object()) {
        for (nlohmann::json::const_iterator it = e.cbegin(); it != e.cend(); it++) {
            expect_equal(it.value(), a[it.key()]);
        }
    } else if (e.is_array()) {
        for (auto i = 0; i < e.size(); i++) {
            expect_equal(e[i], a[i]);
        }
    } else {
        EXPECT_EQ(e, a);
    }
}

void pre_init_core_with_params(core& core, const std::string& config_url) {
    init_core(core, DB_PATH);
    execute_all_core_tasks(core);
    const auto render_task = get_render_task(core);
    send_task(core, render_task["editText"]["valueChangedTask"], {config_url});
    send_task(core, render_task["editText"]["saveValueTask"]);
    core.render_task_queue.clear();
}