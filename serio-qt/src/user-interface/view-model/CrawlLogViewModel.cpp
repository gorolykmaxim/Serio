#include "CrawlLogViewModel.h"
#include <QQmlContext>

serio::qt::CrawlLogViewModel::CrawlLogViewModel(serio::core::TvShowCrawlerEditor& editor,
                                                serio::qt::StackOfViews &stack) : editor(editor), stack(stack) {}

void serio::qt::CrawlLogViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("crawlLogViewModel", this);
    router.registerAction(ActionType::OPEN_PREVIEWED_CRAWLER_LOG, [this] (const QVariantList& args) { openCrawlerPreviewLogView(args); });
    router.registerAction(ActionType::OPEN_CRAWL_LOG_ENTRY_VIEW, [this] (const QVariantList& args) { openLogEntryView(args); });
}

void serio::qt::CrawlLogViewModel::openCrawlerPreviewLogView(const QVariantList& args) {
    QString crawlerType = args[0].toString();
    log = editor.getPreviewedCrawlerLog();
    modifyModel([this, crawlerType] {
        setLogTiles(log);
        setTitle(crawlerType);
        stack.pushView("CrawlLogView.qml");
    });
}

void serio::qt::CrawlLogViewModel::openLogEntryView(const QVariantList& args) {
    unsigned int entryIndex = args[0].toUInt();
    modifyModel([this, entryIndex] {
        selectedEntry = log.at(entryIndex);
        emit selectedEntryChanged();
        stack.pushView("CrawlLogEntryView.qml");
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

void serio::qt::CrawlLogViewModel::setLogTiles(const std::vector<core::CrawlLogEntry> &newLog) {
    logTiles.clearAndDelete();
    emit logChanged();
    for (const auto& entry: newLog) {
        logTiles << new TileModel(QString::fromStdString(entry.getText()));
    }
    emit logChanged();
}

void serio::qt::CrawlLogViewModel::setTitle(const QString& crawlerType) {
    title = crawlerType + " Crawl Log";
    emit titleChanged();
}
