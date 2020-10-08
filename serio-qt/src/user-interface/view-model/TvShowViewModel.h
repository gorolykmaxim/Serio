#ifndef SERIO_TVSHOWVIEWMODEL_H
#define SERIO_TVSHOWVIEWMODEL_H

#include <user-interface/model/pageable-list/EpisodeListModel.h>
#include <user-interface/view-model/DialogViewModel.h>
#include <tv-show-viewer/TvShowViewer.h>
#include <tv-show-crawler-storage/TvShowCrawlerStorage.h>
#include <user-interface/model/ButtonModel.h>
#include "ViewModel.h"
#include "SnackbarViewModel.h"
#include "BackgroundViewModel.h"
#include <user-interface/model/ListModel.h>

namespace serio::qt {

class TvShowViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(QString tvShowName READ getTvShowName NOTIFY selectedTvShowChanged)
    Q_PROPERTY(QString lastWatchDate READ getLastWatchDate NOTIFY selectedTvShowChanged)
    Q_PROPERTY(QString thumbnailUrl READ getThumbnailUrl NOTIFY selectedTvShowChanged)
    Q_PROPERTY(EpisodeListModel* episodeList READ getEpisodeList CONSTANT)
    Q_PROPERTY(QList<ButtonModel*> actions READ getActions NOTIFY actionsChanged)
public:
    TvShowViewModel(unsigned int pageSize, unsigned int pageCountLimit, core::TvShowViewer& viewer,
                    DialogViewModel& dialog, BackgroundViewModel& background, SnackbarViewModel& snackbar,
                    StackOfViews& stack);
    void initialize(ActionRouter& router, QQmlApplicationEngine& engine);
    [[nodiscard]] QString getTvShowName() const;
    [[nodiscard]] QString getLastWatchDate() const;
    [[nodiscard]] QString getThumbnailUrl() const;
    EpisodeListModel* getEpisodeList();
    void load();
    void loadEpisodes(const QVariantList& args);
    void shareCrawler();
    void crawl();
    void confirmClearWatchHistory();
    void clearWatchHistory();
    void confirmDeleteTvShow();
    void deleteTvShow();
    void openTvShowDetails();
    [[nodiscard]] QList<ButtonModel*> getActions() const;
signals:
    void selectedTvShowChanged() const;
    void actionsChanged() const;
private:
    EpisodeListModel episodeListModel;
    QString tvShowName;
    QString thumbnailUrl;
    QString lastWatchDate;
    ListModel<ButtonModel*> actions;
    core::TvShowViewer& viewer;
    DialogViewModel& dialog;
    BackgroundViewModel& background;
    SnackbarViewModel& snackbar;
    StackOfViews& stack;
    void loadTvShow();
    void populateActions(bool includePlayAction);
};

}

#endif //SERIO_TVSHOWVIEWMODEL_H
