#ifndef SERIO_CRAWLEREDITORVIEWMODEL_H
#define SERIO_CRAWLEREDITORVIEWMODEL_H

#include <user-interface/model/TileModel.h>
#include <tv-show-crawler-runtime/model/TvShowCrawler.h>
#include <tv-show-crawler-editor/TvShowCrawlerEditor.h>
#include <user-interface/model/ButtonModel.h>
#include <task-executor/QTaskExecutor.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/model/ListModel.h>
#include <user-interface/action/ActionRouter.h>
#include <QQmlApplicationEngine>
#include "ViewModel.h"

namespace serio::qt {

class CrawlerEditorViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(QString crawlerType READ getCrawlerType NOTIFY crawlerTypeChanged)
    Q_PROPERTY(QList<TileModel*> crawlerSteps READ getCrawlerSteps NOTIFY crawlerStepsChanged)
    Q_PROPERTY(int episodeVideoCrawlerType READ getEpisodeVideoCrawlerType CONSTANT)
    Q_PROPERTY(int thumbnailCrawlerType READ getThumbnailCrawlerType CONSTANT)
    Q_PROPERTY(int episodeNameCrawlerType READ getEpisodeNameCrawlerType CONSTANT)
    Q_PROPERTY(QList<TileModel*> previewResults READ getPreviewResults NOTIFY previewResultsChanged)
    Q_PROPERTY(QList<ButtonModel*> crawlerEditorActions READ getCrawlerEditorActions CONSTANT)
    Q_PROPERTY(QList<ButtonModel*> crawlerPreviewActions READ getCrawlerPreviewActions NOTIFY crawlerPreviewActionsChanged)
public:
    CrawlerEditorViewModel(serio::core::TvShowCrawlerEditor &editor, StackOfViews &stack);
    void initialize(ActionRouter& router, QQmlApplicationEngine& engine);
    [[nodiscard]] QString getCrawlerType() const;
    [[nodiscard]] QList<TileModel*> getCrawlerSteps() const;
    [[nodiscard]] int getEpisodeVideoCrawlerType() const;
    [[nodiscard]] int getThumbnailCrawlerType() const;
    [[nodiscard]] int getEpisodeNameCrawlerType() const;
    [[nodiscard]] QList<TileModel*> getPreviewResults() const;
    void openCrawlerEditor(core::CrawlerType type);
    void openHelp();
    void loadCrawlerSteps();
    void save();
    void openCrawlerPreview();
    [[nodiscard]] QList<ButtonModel*> getCrawlerEditorActions() const;
    [[nodiscard]] QList<ButtonModel*> getCrawlerPreviewActions() const;
signals:
    void crawlerTypeChanged() const;
    void crawlerStepsChanged() const;
    void previewResultsChanged() const;
    void crawlerPreviewActionsChanged() const;
private:
    std::optional<core::CrawlerType> crawlerType;
    ListModel<ButtonModel*> crawlerEditorActions;
    ListModel<ButtonModel*> crawlerPreviewActions;
    ListModel<TileModel*> crawlerSteps;
    ListModel<TileModel*> previewResults;
    core::TvShowCrawlerEditor& editor;
    StackOfViews& stack;
    void setCrawlerSteps(const std::vector<core::CrawlerStep>& steps);
    void setPreviewResults(const std::vector<std::string>& results);
    [[nodiscard]] TileModel* createTileFrom(const core::CrawlerStep& step) const;
    void populateCrawlerPreviewActions();
};

class CrawlerEditorViewNotOpenedError : public std::logic_error {
public:
    CrawlerEditorViewNotOpenedError();
};

}

#endif //SERIO_CRAWLEREDITORVIEWMODEL_H
