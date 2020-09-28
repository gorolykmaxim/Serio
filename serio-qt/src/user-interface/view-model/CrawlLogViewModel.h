#ifndef SERIO_CRAWLLOGVIEWMODEL_H
#define SERIO_CRAWLLOGVIEWMODEL_H

#include <tv-show-crawler-editor/TvShowCrawlerEditor.h>
#include <user-interface/model/TileModel.h>
#include <user-interface/model/ListModel.h>
#include "ViewModel.h"

namespace serio::qt {

class CrawlLogViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(QList<TileModel*> log READ getLog NOTIFY logChanged)
    Q_PROPERTY(QString title READ getTitle NOTIFY titleChanged)
    Q_PROPERTY(QString selectedEntryText READ getSelectedEntryText NOTIFY selectedEntryChanged)
    Q_PROPERTY(QString selectedEntryInputData READ getSelectedEntryInputData NOTIFY selectedEntryChanged)
    Q_PROPERTY(QString selectedEntryOutputData READ getSelectedEntryOutputData NOTIFY selectedEntryChanged)
public:
    CrawlLogViewModel(core::TvShowCrawlerEditor& editor, core::TvShowCrawlerLogStorage& logStorage, StackOfViews& stack);
    void initialize(ActionRouter& router, QQmlApplicationEngine& engine);
    void openCrawlerPreviewLogView(const QVariantList& args);
    void openLastCrawlLogOfTvShow(const QVariantList& args);
    void openLogEntryView(const QVariantList& args);
    [[nodiscard]] QList<TileModel*> getLog() const;
    [[nodiscard]] QString getSelectedEntryText() const;
    [[nodiscard]] QString getSelectedEntryInputData() const;
    [[nodiscard]] QString getSelectedEntryOutputData() const;
    [[nodiscard]] QString getTitle() const;
signals:
    void logChanged();
    void selectedEntryChanged();
    void titleChanged();
private:
    QString title;
    std::vector<core::CrawlLogEntry> logEntries;
    ListModel<TileModel*> logTiles;
    std::optional<core::CrawlLogEntry> selectedEntry;
    core::TvShowCrawlerEditor& editor;
    core::TvShowCrawlerLogStorage& logStorage;
    StackOfViews& stack;
    void openCrawlLogView(const QString& newTitle, std::vector<core::CrawlLogEntry> log);
    void setLogTiles(const std::vector<core::CrawlLogEntry>& newLog);
    void setTitle(QString newTitle);
    void setSelectedEntry(core::CrawlLogEntry newSelectedEntry);
};

}

#endif //SERIO_CRAWLLOGVIEWMODEL_H
