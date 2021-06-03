#include <gtest/gtest.h>
#include <task.h>

TEST(task_test, two_tasks_should_be_equal) {
    task task1{process_http_response};
    task task2{process_http_response};
    EXPECT_EQ(task1, task2);
}