#include <gtest/gtest.h>
#include <queue.h>

TEST(queue_test, should_return_empty_optional_when_try_to_dequeue_from_empty_queue) {
    queue<int> q;
    EXPECT_FALSE(q.try_dequeue());
}

TEST(queue_test, should_deque_previously_enqueued_value) {
    auto value = 15;
    queue<int> q;
    q.enqueue(value);
    EXPECT_EQ(value, *q.try_dequeue());
}

TEST(queue_test, should_dequeue_previously_enqueued_value_waiting_for_it) {
    const auto value = 42;
    queue<int> q;
    std::thread t([&q, value] () {q.enqueue(value);});
    EXPECT_EQ(value, q.dequeue());
    t.join();
}
