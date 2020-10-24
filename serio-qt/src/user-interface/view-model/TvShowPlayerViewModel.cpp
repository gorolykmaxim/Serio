#include <user-interface/ViewNames.h>
#include <QQmlContext>
#include "TvShowPlayerViewModel.h"

namespace serio::qt {

TvShowPlayerViewModel::TvShowPlayerViewModel(core::TvShowPlayer& tvShowPlayer, DialogViewModel& dialog,
                                             StackOfViews &stack)
    : tvShowPlayer(tvShowPlayer), dialog(dialog), stack(stack) {}

void TvShowPlayerViewModel::initialize(ActionRouter &router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("tvShowPlayerViewModel", this);
    router.registerAction(ActionType::PLAY_TV_SHOW, [this] (const auto& args) { playTvShow(args); });
    router.registerAction(ActionType::PLAY_EPISODE_OF_TV_SHOW, [this] (const auto& args) { playEpisodeOfTvShow(args); });
    router.registerAction(ActionType::SET_PLAYING_EPISODE_PROGRESS, [this] (const auto& args) { setProgress(args); });
    router.registerAction(ActionType::PLAY_PREVIOUS_EPISODE, [this] (const auto& args) { playPreviousEpisode(); });
    router.registerAction(ActionType::PLAY_NEXT_EPISODE, [this] (const auto& args) { playNextEpisode(); });
    router.registerAction(ActionType::RE_WATCH_CURRENT_TV_SHOW, [this] (const auto& args) { rewatchCurrentTvShow(); });
}

void TvShowPlayerViewModel::playEpisodeOfTvShow(const QVariantList& args) {
    play(tvShowPlayer.playEpisodeOfTvShow(args[0].toString().toStdString(), args[1].toUInt()));
    stack.pushView(tvShowPlayerView);
}

QString TvShowPlayerViewModel::getEpisodeVideoUrl() const {
    return player ? QString::fromStdString(player->getPlayingEpisode().getVideoUrl()) : "";
}

QString TvShowPlayerViewModel::getTvShowName() const {
    return tvShowName;
}

QString TvShowPlayerViewModel::getEpisodeName() const {
    return player ? QString::fromStdString(player->getPlayingEpisode().getName()) : "";
}

void TvShowPlayerViewModel::setProgress(const QVariantList &args) {
    auto position = args[0].toDouble();
    auto duration = args[1].toDouble();
    if (duration > 0) {
        core::WatchProgress progress(position / duration * 100.0);
        tvShowPlayer.updatePlayingEpisodeWatchProgress(progress);
    }
}

void TvShowPlayerViewModel::playTvShow(const QVariantList &args) {
    tvShowName = args[0].toString();
    player.reset();
    if (tvShowPlayer.isTvShowWatchComplete(tvShowName.toStdString())) {
        displayTvShowIsOverDialog();
    } else {
        play(tvShowPlayer.playTvShow(args[0].toString().toStdString()));
        stack.pushView(tvShowPlayerView);
    }
}

void TvShowPlayerViewModel::play(core::Player &&newPlayer) {
    player = newPlayer;
    tvShowName = QString::fromStdString(player->getPlayingTvShowName());
    emit playingEpisodeChanged();
}

double TvShowPlayerViewModel::getOffsetPercentage() const {
    return player ? player->getStartPercentage() : 0;
}

bool TvShowPlayerViewModel::hasPreviousEpisode() const {
    return player && player->hasPreviousEpisode();
}

bool TvShowPlayerViewModel::hasNextEpisode() const {
    return player && player->hasNextEpisode();
}

void TvShowPlayerViewModel::playPreviousEpisode() {
    play(tvShowPlayer.playPreviousEpisode());
}

void TvShowPlayerViewModel::playNextEpisode() {
    if (hasNextEpisode()) {
        play(tvShowPlayer.playNextEpisode());
    } else {
        displayTvShowIsOverDialog();
    }
}

QString TvShowPlayerViewModel::formatDuration(unsigned int duration) const {
    std::chrono::milliseconds durationMs(duration);
    std::chrono::minutes minutes = std::chrono::duration_cast<std::chrono::minutes>(durationMs);
    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(durationMs - minutes);
    return QString::number(minutes.count()) + ":" + QString::number(seconds.count()).rightJustified(2, QLatin1Char('0'));
}

void TvShowPlayerViewModel::displayTvShowIsOverDialog() {
    DialogModel model("'" + tvShowName + "' is over",
                      "You have watched all available episodes of '" + tvShowName + "'.");
    model.setTopButtonAction(ActionType::RE_WATCH_CURRENT_TV_SHOW);
    model.setTopButtonText("rewatch");
    model.setBottomButtonAction(ActionType::GO_TO_ALL_TV_SHOWS);
    model.setBottomButtonText("watch another");
    dialog.display(model);
}

void TvShowPlayerViewModel::rewatchCurrentTvShow() {
    bool isCurrentlyPlaying = player.has_value();
    play(tvShowPlayer.playEpisodeOfTvShow(tvShowName.toStdString(), 1));
    if (isCurrentlyPlaying) {
        stack.popCurrentView();
    } else {
        stack.replaceCurrentViewWith(tvShowPlayerView);
    }
}

}