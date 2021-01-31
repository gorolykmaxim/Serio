#include <gtest/gtest.h>
#include <localization/Localization.h>

class LocalizationTest : public ::testing::Test {
protected:
    const std::vector<std::string> expectedLanguageNames = {"English", "Русский"};
    const std::vector<serio::Language> expectedLanguages = {
        {expectedLanguageNames[0], {
            {serio::TextId::serio, "Serio"},
            {serio::TextId::daysAgo, "{} days ago"}
        }},
        {expectedLanguageNames[1], {
            {serio::TextId::serio, "Серио"},
            {serio::TextId::daysAgo, "{} дней назад"}
        }}
    };
    serio::Localization localization = serio::Localization(expectedLanguages);
};

TEST_F(LocalizationTest, shouldReturnListOfAvailableLanguages) {
    EXPECT_EQ(expectedLanguageNames, localization.getLanguages());
}

TEST_F(LocalizationTest, shouldReturnEnglishAsDefaultLanguage) {
    EXPECT_EQ(expectedLanguageNames[0], localization.getCurrentLanguage());
}

TEST_F(LocalizationTest, shouldFailToSetCurrentLanguageToUnsupportedOne) {
    EXPECT_THROW(localization.setCurrentLanguage("Unknown"), serio::UnsupportedLanguageError);
}

TEST_F(LocalizationTest, shouldChangeCurrentLanguage) {
    for (const auto& language: localization.getLanguages()) {
        localization.setCurrentLanguage(language);
        EXPECT_EQ(language, localization.getCurrentLanguage());
    }
}

TEST_F(LocalizationTest, shouldFailToGetLocalizedVersionOfTheText) {
    const auto nonExistentTextId = static_cast<serio::TextId>(serio::TextId::serio - 1);
    EXPECT_THROW((void)localization.getText(nonExistentTextId), serio::MissingLocalizationError);
}

TEST_F(LocalizationTest, shouldGetLocalizedVersionOfTheText) {
    const auto expectedText = expectedLanguages[0].idToText.at(serio::TextId::serio);
    EXPECT_EQ(expectedText, localization.getText(serio::TextId::serio));
}

TEST_F(LocalizationTest, shouldFailToGetLocalizedFormatterVersionOfTheTextThatDoesNotHaveParameters) {
    EXPECT_THROW((void)localization.getText(serio::TextId::serio, "data"), serio::NotFormattableTextError);
}

TEST_F(LocalizationTest, shouldGetLocalizedFormattedVersionOfTheText) {
    EXPECT_EQ("2 days ago", localization.getText(serio::TextId::daysAgo, "2"));
    EXPECT_EQ("3 days ago", localization.getText(serio::TextId::daysAgo, "3"));
    localization.setCurrentLanguage(expectedLanguageNames[1]);
    EXPECT_EQ("5 дней назад", localization.getText(serio::TextId::daysAgo, "5"));
}

TEST_F(LocalizationTest, shouldHaveAtLeastOneLanguageByDefault) {
    serio::Localization localization;
    EXPECT_LE(1, localization.getLanguages().size());
}

TEST_F(LocalizationTest, shouldHaveLocalizationForEveryTextIdInEveryAvailableLanguage) {
    serio::Localization localization;
    for (const auto& language: localization.getLanguages()) {
        localization.setCurrentLanguage(language);
        for (int id = serio::TextId::serio; id <= serio::TextId::settings; id++) {
            EXPECT_NO_THROW((void)localization.getText(static_cast<serio::TextId>(id)));
        }
    }
}
