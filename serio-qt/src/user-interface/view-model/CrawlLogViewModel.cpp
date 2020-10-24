#include "CrawlLogViewModel.h"
#include <QQmlContext>
#include <utility>
#include <user-interface/ViewNames.h>

namespace serio::qt {

CrawlLogViewModel::CrawlLogViewModel(core::TvShowCrawlerEditor& editor, core::TvShowCrawlerLogStorage& logStorage,
                                     StackOfViews &stack)
    : editor(editor), logStorage(logStorage), stack(stack) {}

void CrawlLogViewModel::initialize(ActionRouter &router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("crawlLogViewModel", this);
    router.registerAction(ActionType::OPEN_PREVIEWED_CRAWLER_LOG, [this] (const auto& args) { openCrawlerPreviewLogView(args); });
    router.registerAction(ActionType::OPEN_CRAWL_LOG_ENTRY_VIEW, [this] (const auto& args) { openLogEntryView(args); });
    router.registerAction(ActionType::OPEN_LAST_TV_SHOW_CRAWL_LOG, [this] (const auto& args) { openLastCrawlLogOfTvShow(args); });
}

void CrawlLogViewModel::openCrawlerPreviewLogView(const QVariantList& args) {
    auto crawlerType = args[0].toString();
    openCrawlLogView(crawlerType + " Crawl Log", editor.getPreviewedCrawlerLog());
}

void CrawlLogViewModel::openLastCrawlLogOfTvShow(const QVariantList &args) {
    auto tvShowName = args[0].toString().toStdString();
    openCrawlLogView("Last Crawl Log", logStorage.getLastCrawlLogOfTvShow(tvShowName));
}

void CrawlLogViewModel::openLogEntryView(const QVariantList& args) {
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

QList<TileModel *> CrawlLogViewModel::getLog() const {
    return logTiles;
}

QString CrawlLogViewModel::getSelectedEntryText() const {
    return selectedEntry ? QString::fromStdString(selectedEntry->getText()) : QString();
}

QString CrawlLogViewModel::getSelectedEntryInputData() const {
    return selectedEntry ? QString::fromStdString(selectedEntry->getStepInputData()) : QString();
}

QString CrawlLogViewModel::getSelectedEntryOutputData() const {
    return selectedEntry ? QString::fromStdString(selectedEntry->getStepOutputData()) : QString();
}

QString CrawlLogViewModel::getTitle() const {
    return title;
}

void CrawlLogViewModel::openCrawlLogView(const QString &newTitle, std::vector<core::CrawlLogEntry> log) {
    logEntries = std::move(log);
    modifyModel([this, newTitle] {
        setTitle(newTitle);
        setLogTiles(logEntries);
        stack.pushView(crawlLogView);
    });
}

void CrawlLogViewModel::setLogTiles(const std::vector<core::CrawlLogEntry> &newLog) {
    logTiles.clearAndDelete();
    emit logChanged();
    for (const auto& entry: newLog) {
        logTiles << new TileModel(QString::fromStdString(entry.getText()));
    }
    emit logChanged();
}

void CrawlLogViewModel::setTitle(QString newTitle) {
    title = std::move(newTitle);
    emit titleChanged();
}

void CrawlLogViewModel::setSelectedEntry(core::CrawlLogEntry newSelectedEntry) {
    selectedEntry = std::move(newSelectedEntry);
    emit selectedEntryChanged();
}

}