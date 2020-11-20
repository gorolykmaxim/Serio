#include <gtest/gtest.h>
#include <SQLiteCpp/Database.h>
#include <CacheMock.h>
#include <NFClientMock.h>
#include <config/Config.h>

class ConfigTest : public ::testing::Test {
protected:
    const std::string sourceUrl = "https://serio.com/config.json";
    SQLite::Database database = SQLite::Database(":memory:", SQLite::OPEN_READWRITE);
    mocks::CacheMock cache;
    mocks::NFClientMock httpClient;
    serio::Config config = serio::Config(database, cache, httpClient);
};

TEST_F(ConfigTest, shouldSetSourceUrl) {
    EXPECT_FALSE(config.getSourceUrl());
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(sourceUrl, *config.getSourceUrl());
}

TEST_F(ConfigTest, shouldReplaceExistingSourceUrlWithNewOne) {
    config.setSourceUrl("");
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(sourceUrl, config.getSourceUrl());
}

TEST_F(ConfigTest, shouldKeepSourceUrlInDatabase) {
    serio::Config anotherConfig(database, cache, httpClient);
    anotherConfig.setSourceUrl(sourceUrl);
    EXPECT_EQ(sourceUrl, *config.getSourceUrl());
}