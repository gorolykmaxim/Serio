#ifndef SERIO_TVSHOWPLAYERVIEWMODEL_H
#define SERIO_TVSHOWPLAYERVIEWMODEL_H

#include <user-interface/StackOfViews.h>
#include <tv-show-player/TvShowPlayer.h>
#include "ViewModel.h"
#include "DialogViewModel.h"

namespace serio::qt {

class TvShowPlayerViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(QString episodeVideoUrl READ getEpisodeVideoUrl NOTIFY playingEpisodeChanged)
    Q_PROPERTY(QString tvShowName READ getTvShowName NOTIFY playingEpisodeChanged)
    Q_PROPERTY(QString episodeName READ getEpisodeName NOTIFY playingEpisodeChanged)
    Q_PROPERTY(double offsetPercentage READ getOffsetPercentage NOTIFY playingEpisodeChanged)
    Q_PROPERTY(bool hasPreviousEpisode READ hasPreviousEpisode NOTIFY playingEpisodeChanged)
    Q_PROPERTY(bool hasNextEpisode READ hasNextEpisode NOTIFY playingEpisodeChanged)
public:
    TvShowPlayerViewModel(core::TvShowPlayer& tvShowPlayer, DialogViewModel& dialog, StackOfViews& stack);
    void initialize(ActionRouter& router, QQmlApplicationEngine& engine);
    void playEpisodeOfTvShow(const QVariantList& args);
    [[nodiscard]] QString getEpisodeVideoUrl() const;
    [[nodiscard]] QString getTvShowName() const;
    [[nodiscard]] QString getEpisodeName() const;
    void setProgress(const QVariantList& args);
    void playTvShow(const QVariantList& args);
    [[nodiscard]] double getOffsetPercentage() const;
    [[nodiscard]] bool hasPreviousEpisode() const;
    [[nodiscard]] bool hasNextEpisode() const;
    void playPreviousEpisode();
    void playNextEpisode();
    void rewatchCurrentTvShow();
public slots:
    [[nodiscard]] QString formatDuration(unsigned int duration) const;
signals:
    void playingEpisodeChanged() const;
private:
    core::TvShowPlayer& tvShowPlayer;
    QString tvShowName;
    std::optional<core::Player> player;
    DialogViewModel& dialog;
    StackOfViews& stack;
    void play(core::Player&& newPlayer);
    void displayTvShowIsOverDialog();
};

}

#endif //SERIO_TVSHOWPLAYERVIEWMODEL_H
