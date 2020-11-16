#include <gtest/gtest.h>
#include <cache/Cache.h>

class CacheTest : public ::testing::Test {
protected:
    const std::string key = "key";
    const std::string value = "value to cache";
    SQLite::Database database = SQLite::Database(":memory:", SQLite::OPEN_READWRITE);
    serio::Cache cache = serio::Cache(database);
};

TEST_F(CacheTest, shouldMissCache) {
    EXPECT_FALSE(cache.get(key));
}

TEST_F(CacheTest, shouldHitCache) {
    cache.put(key, value, std::chrono::hours(24));
    EXPECT_EQ(value, *cache.get(key));
}

TEST_F(CacheTest, shouldMissCacheSinceLastEntryHasAlreadyExpired) {
    cache.put(key, value, std::chrono::hours(-24));
    EXPECT_FALSE(cache.get(key));
}

TEST_F(CacheTest, shouldAutomaticallyCleanCacheFromExpiredEntries) {
    cache.put(key, value, std::chrono::hours(-24));
    EXPECT_EQ(1, cache.size());
    cache.get(key);
    EXPECT_EQ(0, cache.size());
}
