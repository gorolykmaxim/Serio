#include <gtest/gtest.h>
#include <tv-show-crawler-runtime/model/CrawlerStepType.h>

class CrawlerStepTypeTest : public ::testing::Test {
protected:
    std::string description = "Description of a crawler step type";
};

TEST_F(CrawlerStepTypeTest, shoudlHaveSpecifiedName) {
    serio::core::CrawlerStepType type("fetch", "");
    EXPECT_EQ("fetch", type.getName());
    type = serio::core::CrawlerStepType("value", "");
    EXPECT_EQ("value", type.getName());
}

TEST_F(CrawlerStepTypeTest, shouldHaveSpecifiedDescription) {
    for (const auto& description: {"Description of the step type", "Different step type description"}) {
        serio::core::CrawlerStepType type("fetch", description);
        EXPECT_EQ(description, type.getDescription());
    }
}

TEST_F(CrawlerStepTypeTest, shouldNotHaveMandatoryProperties) {
    serio::core::CrawlerStepType type("fetch", description);
    EXPECT_TRUE(type.getMandatoryProperties().empty());
}

TEST_F(CrawlerStepTypeTest, shouldHaveSpecifiedMandatoryProperties) {
    std::vector<std::string> mandatoryProperties = {"a", "b"};
    serio::core::CrawlerStepType type("calc", description, mandatoryProperties);
    EXPECT_EQ(mandatoryProperties, type.getMandatoryProperties());
}

TEST_F(CrawlerStepTypeTest, twoCrawlerStepTypesWithTheSameNameAndMandatoryPropertiesShouldBeEqual) {
    EXPECT_EQ(serio::core::CrawlerStepType("value", description, {"value"}),
              serio::core::CrawlerStepType("value", description, {"value"}));
}

TEST_F(CrawlerStepTypeTest, twoCrawlerStepTypesWithDifferentNamesOrMandatoryPropertiesShouldNotBeEqual) {
    serio::core::CrawlerStepType type("regExp", description, {"regExp"});
    EXPECT_NE(type, serio::core::CrawlerStepType("regExp", description));
    EXPECT_NE(type, serio::core::CrawlerStepType("value", description, {"value"}));
    EXPECT_NE(type, serio::core::CrawlerStepType("regExp", "", {"regExp"}));
}
