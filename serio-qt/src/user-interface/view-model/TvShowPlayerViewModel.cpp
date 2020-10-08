#include <user-interface/ViewNames.h>
#include <QQmlContext>
#include "TvShowPlayerViewModel.h"

serio::qt::TvShowPlayerViewModel::TvShowPlayerViewModel(serio::core::TvShowPlayer& tvShowPlayer,
                                                        serio::qt::StackOfViews &stack)
    : tvShowPlayer(tvShowPlayer), stack(stack) {}

void serio::qt::TvShowPlayerViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("tvShowPlayerViewModel", this);
    router.registerAction(ActionType::PLAY_TV_SHOW, [this] (const auto& args) { playTvShow(args); });
    router.registerAction(ActionType::PLAY_EPISODE_OF_TV_SHOW, [this] (const auto& args) { playEpisodeOfTvShow(args); });
    router.registerAction(ActionType::SET_PLAYING_EPISODE_PROGRESS, [this] (const auto& args) { setProgress(args); });
}

void serio::qt::TvShowPlayerViewModel::playEpisodeOfTvShow(const QVariantList& args) {
    play(tvShowPlayer.playEpisodeOfTvShow(args[0].toString().toStdString(), args[1].toUInt()));
}

QString serio::qt::TvShowPlayerViewModel::getEpisodeVideoUrl() const {
    return player ? QString::fromStdString(player->getPlayingEpisode().getVideoUrl()) : "";
}

QString serio::qt::TvShowPlayerViewModel::getTvShowName() const {
    return player ? QString::fromStdString(player->getPlayingTvShowName()) : "";
}

QString serio::qt::TvShowPlayerViewModel::getEpisodeName() const {
    return player ? QString::fromStdString(player->getPlayingEpisode().getName()) : "";
}

void serio::qt::TvShowPlayerViewModel::setProgress(const QVariantList &args) {
    auto position = args[0].toDouble();
    auto duration = args[1].toDouble();
    if (duration > 0) {
        serio::core::WatchProgress progress(position / duration * 100.0);
        tvShowPlayer.updatePlayingEpisodeWatchProgress(progress);
    }
}

void serio::qt::TvShowPlayerViewModel::playTvShow(const QVariantList &args) {
    play(tvShowPlayer.playTvShow(args[0].toString().toStdString()));
}

void serio::qt::TvShowPlayerViewModel::play(serio::core::Player &&newPlayer) {
    player = newPlayer;
    emit playingEpisodeChanged();
    stack.pushView(tvShowPlayerView);
}

double serio::qt::TvShowPlayerViewModel::getOffsetPercentage() const {
    return player ? player->getStartPercentage() : 0;
}
