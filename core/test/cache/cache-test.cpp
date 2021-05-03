#include <gtest/gtest.h>
#include "cache/cache.h"

class cache_test : public ::testing::Test {
protected:
    const std::string key = "key";
    const std::string value = "value";
    const std::chrono::hours duration = std::chrono::hours(24);
    SQLite::Database database = SQLite::Database(":memory:", SQLite::OPEN_READWRITE);

    virtual void SetUp() {
        serio::cache::init(database);
    }
};

TEST_F(cache_test, should_miss_cache) {
    EXPECT_FALSE(serio::cache::get(database, key));
}

TEST_F(cache_test, should_hit_cache) {
    serio::cache::put(database, key, value, duration);
    EXPECT_EQ(value, *serio::cache::get(database, key));
}

TEST_F(cache_test, should_miss_cache_if_key_is_different_from_stored_one) {
    serio::cache::put(database, "another " + key, value, duration);
    EXPECT_FALSE(serio::cache::get(database, key));
}

TEST_F(cache_test, should_miss_cache_since_last_entry_has_already_expired) {
    serio::cache::put(database, key, value, std::chrono::hours(-24));
    EXPECT_FALSE(serio::cache::get(database, key));
}

TEST_F(cache_test, should_replace_existing_cache_entry_with_new_one) {
    serio::cache::put(database, key, "", duration);
    serio::cache::put(database, key, value, duration);
    EXPECT_EQ(value, *serio::cache::get(database, key));
}

TEST_F(cache_test, should_get_last_expired_entry_from_cache) {
    serio::cache::put(database, key, value, std::chrono::hours(-24));
    EXPECT_EQ(value, *serio::cache::get(database, key, true));
}
