#include <gtest/gtest.h>
#include <error.h>

TEST(error_test, should_ignore_non_error_tasks) {
    const task task{init};
    trigger_fatal_error(task);
}

TEST(error_test, should_throw_exception_with_message_specified_in_error_task) {
    const std::string error = "Error message";
    const task task{task_type::error, {error}};
    try {
        trigger_fatal_error(task);
        FAIL() << "Should throw exception with specified message";
    } catch (const std::exception& e) {
        EXPECT_EQ(e.what(), error);
    }
}

TEST(error_test, should_throw_exception_with_no_message_specified_in_error_task) {
    try {
        trigger_fatal_error({error});
        FAIL() << "Should throw exception with no message";
    } catch (const std::exception& e) {
        EXPECT_EQ(e.what(), std::string("no error message ") + u8"\U0001F602");
    }
}
