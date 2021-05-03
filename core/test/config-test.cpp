#include <gtest/gtest.h>
#include <SQLiteCpp/Database.h>
#include <config.h>

class config_test : public ::testing::Test {
protected:
    const std::string value = "https://github.com/serio/data/config.json";
    SQLite::Database database = SQLite::Database(":memory:", SQLite::OPEN_READWRITE);

    virtual void SetUp() {
        init_config(database);
    }
};

TEST_F(config_test, should_not_find_specified_property) {
    EXPECT_FALSE(get_config_property(database, CONFIG_SOURCE_URL));
}

TEST_F(config_test, should_store_specified_property) {
    set_config_property(database, CONFIG_SOURCE_URL, value);
    EXPECT_EQ(value, *get_config_property(database, CONFIG_SOURCE_URL));
}

TEST_F(config_test, should_not_find_property_with_different_name) {
    set_config_property(database, "another-" + CONFIG_SOURCE_URL, value);
    EXPECT_FALSE(get_config_property(database, CONFIG_SOURCE_URL));
}

TEST_F(config_test, should_replace_existing_property_value_with_new_one) {
    set_config_property(database, CONFIG_SOURCE_URL, "");
    set_config_property(database, CONFIG_SOURCE_URL, value);
    EXPECT_EQ(value, *get_config_property(database, CONFIG_SOURCE_URL));
}
