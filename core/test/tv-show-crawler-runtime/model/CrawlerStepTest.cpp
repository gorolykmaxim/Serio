#include <gtest/gtest.h>
#include <tv-show-crawler-runtime/model/CrawlerStep.h>

TEST(CrawlerStepTest, shouldCreateCrawlerStepWithTheSpecifiedType) {
    serio::core::CrawlerStep step("fetch");
    EXPECT_EQ("fetch", step.getType());
    step = serio::core::CrawlerStep("value");
    EXPECT_EQ("value", step.getType());
}

TEST(CrawlerStepTest, shouldNotHaveSpecifiedProperty) {
    serio::core::CrawlerStep step("transform");
    EXPECT_FALSE(step.getProperty("template"));
}

TEST(CrawlerStepTest, shouldHaveSpecifiedProperty) {
    serio::core::CrawlerStep step("value", {{"value", "42"}});
    EXPECT_EQ("42", step.getProperty("value"));
}

TEST(CrawlerStepTest, shouldReturnAllOfItsProperties) {
    std::map<std::string, std::string> properties = {{"regExp", "(.*)"}};
    serio::core::CrawlerStep step("regExp", properties);
    EXPECT_EQ(properties, step.getProperties());
}

TEST(CrawlerStepTest, twoStepsShouldBeEqualIfTheirTypesAndPropertiesSame) {
    EXPECT_EQ(serio::core::CrawlerStep("value", {{"value", "v"}}),
              serio::core::CrawlerStep("value", {{"value", "v"}}));
}

TEST(CrawlerStepTest, twoStepsShouldNotBeEqualIfTheirTypesOrPropertiesAreDifferent) {
    serio::core::CrawlerStep step("fetch");
    EXPECT_NE(step, serio::core::CrawlerStep("value"));
    EXPECT_NE(step, serio::core::CrawlerStep("fetch", {{"url", "https://localhost"}}));
}

TEST(CrawlerStepTest, shouldFailToGetPropertyStepDoesNotHave) {
    serio::core::CrawlerStep step("fetch");
    EXPECT_THROW((void)step.getPropertyOrFail("template"), std::invalid_argument);
}

TEST(CrawlerStepTest, shouldGetPropertyStepHas) {
    serio::core::CrawlerStep step("regExp", {{"regExp", "(.*)"}});
    EXPECT_EQ("(.*)", step.getPropertyOrFail("regExp"));
}

TEST(CrawlerStepTest, shouldReturnEmptyStringDueToNoProperties) {
    EXPECT_TRUE(serio::core::CrawlerStep("fetch").getPropertiesAsString().empty());
}

TEST(CrawlerStepTest, shouldReturnPropertiesAsString) {
    serio::core::CrawlerStep step("abc", {{"a", "text"}, {"b", "15"}, {"c", "true"}});
    EXPECT_EQ("a: text, b: 15, c: true", step.getPropertiesAsString());
}
