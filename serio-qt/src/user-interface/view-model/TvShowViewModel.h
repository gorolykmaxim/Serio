#ifndef SERIO_TVSHOWVIEWMODEL_H
#define SERIO_TVSHOWVIEWMODEL_H

#include <user-interface/model/pageable-list/EpisodeListModel.h>
#include <tv-show-viewer/TvShowViewer.h>
#include <tv-show-crawler-storage/TvShowCrawlerStorage.h>
#include "ViewModel.h"
#include "SnackbarViewModel.h"

namespace serio::qt {

class TvShowViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(QString tvShowName READ getTvShowName NOTIFY selectedTvShowChanged)
    Q_PROPERTY(QString lastWatchDate READ getLastWatchDate NOTIFY selectedTvShowChanged)
    Q_PROPERTY(QString thumbnailUrl READ getThumbnailUrl NOTIFY selectedTvShowChanged)
    Q_PROPERTY(EpisodeListModel* episodeList READ getEpisodeList CONSTANT)
public:
    TvShowViewModel(unsigned int pageSize, unsigned int pageCountLimit, core::TvShowViewer& viewer, SnackbarViewModel& snackbar);
    void initialize(ActionRouter& router, QQmlApplicationEngine& engine);
    [[nodiscard]] QString getTvShowName() const;
    [[nodiscard]] QString getLastWatchDate() const;
    [[nodiscard]] QString getThumbnailUrl() const;
    EpisodeListModel* getEpisodeList();
    void load();
    void loadEpisodes(const QVariantList& args);
    void shareCrawler();
signals:
    void selectedTvShowChanged();
private:
    EpisodeListModel episodeListModel;
    QString tvShowName;
    QString thumbnailUrl;
    QString lastWatchDate;
    core::TvShowViewer& viewer;
    SnackbarViewModel& snackbar;
    void loadTvShow();
};

}

#endif //SERIO_TVSHOWVIEWMODEL_H
