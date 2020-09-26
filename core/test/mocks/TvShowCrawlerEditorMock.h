#ifndef SERIO_TVSHOWCRAWLEREDITORMOCK_H
#define SERIO_TVSHOWCRAWLEREDITORMOCK_H

#include <gmock/gmock.h>
#include <tv-show-crawler-editor/TvShowCrawlerEditor.h>
#include "TvShowCrawlerRuntimeMock.h"

class TvShowCrawlerEditorMock : public serio::core::TvShowCrawlerEditor {
public:
    static TvShowCrawlerEditorMock create() {
        TvShowCrawlerRuntimeMock runtime = TvShowCrawlerRuntimeMock::create();
        return TvShowCrawlerEditorMock(runtime);
    }
    explicit TvShowCrawlerEditorMock(serio::core::TvShowCrawlerRuntime& runtime) : TvShowCrawlerEditor(runtime) {}
    MOCK_METHOD(void, createTvShowCrawler, (), (override));
    MOCK_METHOD(void, importTvShowCrawler, (const std::string&), (override));
    MOCK_METHOD(void, editTvShowCrawler, (const std::string&), (override));
    MOCK_METHOD(void, setTvShowName, (std::string), (override));
    MOCK_METHOD(void, editCrawler, (serio::core::CrawlerType), (override));
    MOCK_METHOD(void, addCrawlerStep, (serio::core::CrawlerStep), (override));
    MOCK_METHOD(void, replaceCrawlerStep, (unsigned int, serio::core::CrawlerStep), (override));
    MOCK_METHOD(void, removeCrawlerStep, (unsigned int), (override));
    MOCK_METHOD(void, saveCrawler, (), (override));
    MOCK_METHOD(void, saveAndRunTvShowCrawler, (), (override));
    MOCK_METHOD(serio::core::CrawlResult, previewCrawler, (), (override));
    MOCK_METHOD(bool, willOverrideExistingTvShow, (), (override));
    MOCK_METHOD(std::string, getTvShowName, (), (const override));
    MOCK_METHOD((std::vector<serio::core::CrawlerStep>), getCrawlerSteps, (), (const override));
    MOCK_METHOD((std::vector<serio::core::CrawlerStepType>), getCrawlerStepTypes, (), (const override));
    MOCK_METHOD((std::vector<serio::core::CrawlLogEntry>), getPreviewedCrawlerLog, (), (const override));
};

#endif //SERIO_TVSHOWCRAWLEREDITORMOCK_H
