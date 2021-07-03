#include <gtest/gtest.h>
#include <task.h>

TEST(task_test, two_tasks_should_be_equal) {
    task task1{process_http_response};
    task task2{process_http_response};
    EXPECT_EQ(task1, task2);
}

TEST(task_test, two_tasks_should_not_be_equal) {
    const task task1{init};
    const task task2{init, {15}};
    EXPECT_NE(task1, task2);
}

TEST(task_test, should_parse_task_with_args) {
    const task expected{process_http_response, {"a", 1, true}};
    const nlohmann::json raw_task = {{"taskId", process_http_response}, {"args", expected.args}};
    const auto actual = parse_task(raw_task);
    EXPECT_EQ(expected, actual);
}
