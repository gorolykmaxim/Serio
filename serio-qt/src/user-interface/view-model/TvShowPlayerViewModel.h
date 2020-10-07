#ifndef SERIO_TVSHOWPLAYERVIEWMODEL_H
#define SERIO_TVSHOWPLAYERVIEWMODEL_H

#include <user-interface/StackOfViews.h>
#include <tv-show-player/TvShowPlayer.h>
#include "ViewModel.h"

namespace serio::qt {

class TvShowPlayerViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(QString episodeVideoUrl READ getEpisodeVideoUrl NOTIFY playingEpisodeChanged)
    Q_PROPERTY(QString tvShowName READ getTvShowName NOTIFY playingEpisodeChanged)
    Q_PROPERTY(QString episodeName READ getEpisodeName NOTIFY playingEpisodeChanged)
public:
    TvShowPlayerViewModel(core::TvShowPlayer& tvShowPlayer, StackOfViews& stack);
    void initialize(ActionRouter& router, QQmlApplicationEngine& engine);
    void playEpisodeOfTvShow(const QVariantList& args);
    [[nodiscard]] QString getEpisodeVideoUrl() const;
    [[nodiscard]] QString getTvShowName() const;
    [[nodiscard]] QString getEpisodeName() const;
    void setProgress(const QVariantList& args);
signals:
    void playingEpisodeChanged() const;
private:
    core::TvShowPlayer& tvShowPlayer;
    std::optional<core::Player> player;
    StackOfViews& stack;
};

}

#endif //SERIO_TVSHOWPLAYERVIEWMODEL_H
