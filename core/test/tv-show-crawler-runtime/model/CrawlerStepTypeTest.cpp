#include <gtest/gtest.h>
#include <tv-show-crawler-runtime/model/CrawlerStepType.h>

TEST(CrawlerStepTypeTest, shoudlHaveSpecifiedName) {
    serio::core::CrawlerStepType type("fetch");
    EXPECT_EQ("fetch", type.getName());
    type = serio::core::CrawlerStepType("value");
    EXPECT_EQ("value", type.getName());
}

TEST(CrawlerStepTypeTest, shouldNotHaveMandatoryProperties) {
    serio::core::CrawlerStepType type("fetch");
    EXPECT_TRUE(type.getMandatoryProperties().empty());
}

TEST(CrawlerStepTypeTest, shouldHaveSpecifiedMandatoryProperties) {
    std::vector<std::string> mandatoryProperties = {"a", "b"};
    serio::core::CrawlerStepType type("calc", mandatoryProperties);
    EXPECT_EQ(mandatoryProperties, type.getMandatoryProperties());
}

TEST(CrawlerStepTypeTest, twoCrawlerStepTypesWithTheSameNameAndMandatoryPropertiesShouldBeEqual) {
    EXPECT_EQ(serio::core::CrawlerStepType("value", {"value"}),
              serio::core::CrawlerStepType("value", {"value"}));
}

TEST(CrawlerStepTypeTest, twoCrawlerStepTypesWithDifferentNamesOrMandatoryPropertiesShouldNotBeEqual) {
    serio::core::CrawlerStepType type("regExp", {"regExp"});
    EXPECT_NE(type, serio::core::CrawlerStepType("regExp"));
    EXPECT_NE(type, serio::core::CrawlerStepType("value", {"value"}));
}
