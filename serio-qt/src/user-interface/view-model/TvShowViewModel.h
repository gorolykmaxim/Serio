#ifndef SERIO_TVSHOWVIEWMODEL_H
#define SERIO_TVSHOWVIEWMODEL_H

#include <tv-show-storage/TvShowStorage.h>
#include <user-interface/model/pageable-list/EpisodeListModel.h>
#include "ViewModel.h"

namespace serio::qt {

class TvShowViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(QString tvShowName READ getTvShowName NOTIFY selectedTvShowChanged)
    Q_PROPERTY(QString lastWatchDate READ getLastWatchDate NOTIFY selectedTvShowChanged)
    Q_PROPERTY(QString thumbnailUrl READ getThumbnailUrl NOTIFY selectedTvShowChanged)
    Q_PROPERTY(EpisodeListModel* episodeList READ getEpisodeList CONSTANT)
public:
    TvShowViewModel(unsigned int pageSize, unsigned int pageCountLimit, core::TvShowStorage& storage, StackOfViews& stack);
    void initialize(ActionRouter& router, QQmlApplicationEngine& engine);
    [[nodiscard]] QString getTvShowName() const;
    [[nodiscard]] QString getLastWatchDate() const;
    [[nodiscard]] QString getThumbnailUrl() const;
    EpisodeListModel* getEpisodeList();
    void openView(const QList<QVariant> &args);
    void load();
    void loadEpisodes(const QVariantList& args);
signals:
    void selectedTvShowChanged();
private:
    const unsigned int pageSize;
    EpisodeListModel episodeListModel;
    core::TvShowStorage& storage;
    StackOfViews& stack;
    std::optional<QString> selectedTvShowName;
    std::optional<core::TvShow> selectedTvShow;
    void loadTvShow();
};

}

#endif //SERIO_TVSHOWVIEWMODEL_H
