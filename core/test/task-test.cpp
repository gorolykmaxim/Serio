#include <gtest/gtest.h>
#include <task.h>

class task_test : public ::testing::Test {
protected:
    queue<std::string> queue;
};

TEST_F(task_test, two_tasks_should_be_equal) {
    task task1{process_http_response};
    task task2{process_http_response};
    EXPECT_EQ(task1, task2);
}

TEST_F(task_test, two_tasks_should_not_be_equal) {
    const task task1{init};
    const task task2{init, {15}};
    EXPECT_NE(task1, task2);
}

TEST_F(task_test, should_deserialize_task_from_json_with_empty_args_if_args_not_specified) {
    const task expected{init};
    const nlohmann::json raw_task = {{"event", init}};
    queue.enqueue(raw_task.dump());
    const auto actual = read_task(queue);
    EXPECT_EQ(expected, actual);
}

TEST_F(task_test, should_deserialize_task_from_json_with_specified_args) {
    const nlohmann::json args = {"arg 1", 2, true};
    const nlohmann::json raw_task = {{"event", error}, {"args", args}};
    const task expected = {error, args};
    queue.enqueue(raw_task.dump());
    const auto actual = read_task(queue);
    EXPECT_EQ(expected, actual);
}

TEST_F(task_test, should_fail_to_deserialize_task_that_has_event_in_wrong_type) {
    const nlohmann::json raw_task = {{"event", "wrong"}};
    queue.enqueue(raw_task.dump());
    EXPECT_THROW(read_task(queue), task_parse_error);
}

TEST_F(task_test, should_fail_to_deserialize_task_that_has_args_in_wrong_type) {
    const nlohmann::json raw_task = {{"event", 1}, {"args", {{"a", 1}, {"b", "c"}}}};
    queue.enqueue(raw_task.dump());
    EXPECT_THROW(read_task(queue), task_parse_error);
}
