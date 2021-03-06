#ifndef SERIO_QUEUE_H
#define SERIO_QUEUE_H

#include <blockingconcurrentqueue.h>
#include <optional>

template<typename T>
struct queue {
public:
    void enqueue(T t) {
        q.enqueue(std::move(t));
    }
    std::optional<T> try_dequeue() {
        T t;
        return q.try_dequeue(t) ? t : std::optional<T>();
    }
    T dequeue() {
        T t;
        q.wait_dequeue(t);
        return t;
    }
    bool empty() {
        return q.size_approx() == 0;
    }
    void clear() {
        T t;
        while (q.try_dequeue(t));
    }
private:
    moodycamel::BlockingConcurrentQueue<T> q;
};

#endif //SERIO_QUEUE_H
