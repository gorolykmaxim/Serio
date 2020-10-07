#include "CrawlLogViewModel.h"
#include <QQmlContext>
#include <utility>
#include <user-interface/ViewNames.h>

serio::qt::CrawlLogViewModel::CrawlLogViewModel(serio::core::TvShowCrawlerEditor& editor,
                                                serio::core::TvShowCrawlerLogStorage& logStorage,
                                                serio::qt::StackOfViews &stack)
    : editor(editor), logStorage(logStorage), stack(stack) {}

void serio::qt::CrawlLogViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("crawlLogViewModel", this);
    router.registerAction(ActionType::OPEN_PREVIEWED_CRAWLER_LOG, [this] (const auto& args) { openCrawlerPreviewLogView(args); });
    router.registerAction(ActionType::OPEN_CRAWL_LOG_ENTRY_VIEW, [this] (const auto& args) { openLogEntryView(args); });
    router.registerAction(ActionType::OPEN_LAST_TV_SHOW_CRAWL_LOG, [this] (const auto& args) { openLastCrawlLogOfTvShow(args); });
}

void serio::qt::CrawlLogViewModel::openCrawlerPreviewLogView(const QVariantList& args) {
    auto crawlerType = args[0].toString();
    openCrawlLogView(crawlerType + " Crawl Log", editor.getPreviewedCrawlerLog());
}

void serio::qt::CrawlLogViewModel::openLastCrawlLogOfTvShow(const QVariantList &args) {
    auto tvShowName = args[0].toString().toStdString();
    openCrawlLogView("Last Crawl Log", logStorage.getLastCrawlLogOfTvShow(tvShowName));
}

void serio::qt::CrawlLogViewModel::openLogEntryView(const QVariantList& args) {
    auto entryIndex = args[0].toUInt();
    auto entry = logEntries.at(entryIndex);
    if (!entry.hasData()) {
        return;
    }
    modifyModel([this, entry] {
        setSelectedEntry(entry);
        stack.pushView(crawlLogEntryView);
    });
}

QList<serio::qt::TileModel *> serio::qt::CrawlLogViewModel::getLog() const {
    return logTiles;
}

QString serio::qt::CrawlLogViewModel::getSelectedEntryText() const {
    return selectedEntry ? QString::fromStdString(selectedEntry->getText()) : QString();
}

QString serio::qt::CrawlLogViewModel::getSelectedEntryInputData() const {
    return selectedEntry ? QString::fromStdString(selectedEntry->getStepInputData()) : QString();
}

QString serio::qt::CrawlLogViewModel::getSelectedEntryOutputData() const {
    return selectedEntry ? QString::fromStdString(selectedEntry->getStepOutputData()) : QString();
}

QString serio::qt::CrawlLogViewModel::getTitle() const {
    return title;
}

void serio::qt::CrawlLogViewModel::openCrawlLogView(const QString &newTitle, std::vector<core::CrawlLogEntry> log) {
    logEntries = std::move(log);
    modifyModel([this, newTitle] {
        setTitle(newTitle);
        setLogTiles(logEntries);
        stack.pushView(crawlLogView);
    });
}

void serio::qt::CrawlLogViewModel::setLogTiles(const std::vector<core::CrawlLogEntry> &newLog) {
    logTiles.clearAndDelete();
    emit logChanged();
    for (const auto& entry: newLog) {
        logTiles << new TileModel(QString::fromStdString(entry.getText()));
    }
    emit logChanged();
}

void serio::qt::CrawlLogViewModel::setTitle(QString newTitle) {
    title = std::move(newTitle);
    emit titleChanged();
}

void serio::qt::CrawlLogViewModel::setSelectedEntry(serio::core::CrawlLogEntry newSelectedEntry) {
    selectedEntry = std::move(newSelectedEntry);
    emit selectedEntryChanged();
}
