#include <gtest/gtest.h>
#include <localization/Localization.h>
#include <ConfigMock.h>

class LanguageStorageMock : public mocks::ConfigMock {
public:
    LanguageStorageMock() = default;
    explicit LanguageStorageMock(const std::string& currentLanguage) {
        properties[serio::Localization::CURRENT_LANGUAGE_CONFIG_PROPERTY] = currentLanguage;
    }
    void setProperty(const std::string& name, const std::string& value) override {
        properties[name] = value;
    }
    std::optional<std::string> getProperty(const std::string& name) override {
        const auto it = properties.find(name);
        if (it != properties.cend()) {
            return it->second;
        } else {
            return std::optional<std::string>();
        }
    }
private:
    std::unordered_map<std::string, std::string> properties;
};

class LocalizationTest : public ::testing::Test {
protected:
    const std::vector<std::string> expectedLanguageNames = {"English", "Русский"};
    const std::vector<serio::Translation> expectedTranslations = {
            {expectedLanguageNames[0], serio::TextId::serio, "Serio"},
            {expectedLanguageNames[0], serio::TextId::daysAgo, "{} days ago"},
            {expectedLanguageNames[1], serio::TextId::serio, "Серио"},
            {expectedLanguageNames[1], serio::TextId::daysAgo, "{} день назад", "1"},
            {expectedLanguageNames[1], serio::TextId::daysAgo, "{} дней назад"},
            {expectedLanguageNames[1], serio::TextId::monthsAgo, "{} месяц назад", "1"},
    };
    LanguageStorageMock config;
    serio::Localization localization = serio::Localization(expectedTranslations, config);
};

TEST_F(LocalizationTest, shouldFailToConstructLocalizationWithNoTraslationsProvided) {
    EXPECT_THROW(serio::Localization({}, config), serio::NoTranslationsProvidedError);
}

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

TEST_F(LocalizationTest, shouldReadCurrentLanguageFromConfigOnInstantiation) {
    LanguageStorageMock config(expectedLanguageNames[1]);
    serio::Localization localization(expectedTranslations, config);
    EXPECT_EQ(expectedLanguageNames[1], localization.getCurrentLanguage());
}

TEST_F(LocalizationTest, shouldFallbackToEnglishAsCurrentLanguageIfLanguageStoredInConfigDoesNotExist) {
    LanguageStorageMock config("not supported language");
    serio::Localization localization(expectedTranslations, config);
    EXPECT_EQ(expectedLanguageNames[0], localization.getCurrentLanguage());
}

TEST_F(LocalizationTest, shouldFailToGetLocalizedVersionOfTheText) {
    const auto nonExistentTextId = static_cast<serio::TextId>(serio::TextId::serio - 1);
    EXPECT_THROW((void)localization.getText(nonExistentTextId), serio::MissingLocalizationError);
}

TEST_F(LocalizationTest, shouldGetLocalizedVersionOfTheText) {
    const auto expectedText = expectedTranslations[0].text;
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
    EXPECT_EQ("1 день назад", localization.getText(serio::TextId::daysAgo, "1"));
}

TEST_F(LocalizationTest, shouldFailToGetLocalizedTextForParameterThatDoesNotMatchAnyParameterRule) {
    localization.setCurrentLanguage(expectedLanguageNames[1]);
    EXPECT_THROW((void)localization.getText(serio::TextId::monthsAgo, "3"), serio::MissingLocalizationError);
}

TEST_F(LocalizationTest, shouldHaveAtLeastOneLanguageByDefault) {
    serio::Localization localization(config);
    EXPECT_LE(1, localization.getLanguages().size());
}

TEST_F(LocalizationTest, shouldHaveLocalizationForEveryTextIdInEveryAvailableLanguage) {
    serio::Localization localization(config);
    for (const auto& language: localization.getLanguages()) {
        localization.setCurrentLanguage(language);
        for (int id = serio::TextId::serio; id <= serio::TextId::settings; id++) {
            EXPECT_NO_THROW((void)localization.getText(static_cast<serio::TextId>(id)));
        }
    }
}
