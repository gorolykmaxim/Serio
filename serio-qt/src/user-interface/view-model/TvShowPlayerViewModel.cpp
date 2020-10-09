#include <user-interface/ViewNames.h>
#include <QQmlContext>
#include "TvShowPlayerViewModel.h"

serio::qt::TvShowPlayerViewModel::TvShowPlayerViewModel(serio::core::TvShowPlayer& tvShowPlayer,
                                                        serio::qt::DialogViewModel& dialog,
                                                        serio::qt::StackOfViews &stack)
    : tvShowPlayer(tvShowPlayer), dialog(dialog), stack(stack) {}

void serio::qt::TvShowPlayerViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("tvShowPlayerViewModel", this);
    router.registerAction(ActionType::PLAY_TV_SHOW, [this] (const auto& args) { playTvShow(args); });
    router.registerAction(ActionType::PLAY_EPISODE_OF_TV_SHOW, [this] (const auto& args) { playEpisodeOfTvShow(args); });
    router.registerAction(ActionType::SET_PLAYING_EPISODE_PROGRESS, [this] (const auto& args) { setProgress(args); });
    router.registerAction(ActionType::PLAY_PREVIOUS_EPISODE, [this] (const auto& args) { playPreviousEpisode(); });
    router.registerAction(ActionType::PLAY_NEXT_EPISODE, [this] (const auto& args) { playNextEpisode(); });
    router.registerAction(ActionType::RE_WATCH_CURRENT_TV_SHOW, [this] (const auto& args) { rewatchCurrentTvShow(); });
}

void serio::qt::TvShowPlayerViewModel::playEpisodeOfTvShow(const QVariantList& args) {
    play(tvShowPlayer.playEpisodeOfTvShow(args[0].toString().toStdString(), args[1].toUInt()));
    stack.pushView(tvShowPlayerView);
}

QString serio::qt::TvShowPlayerViewModel::getEpisodeVideoUrl() const {
    return player ? QString::fromStdString(player->getPlayingEpisode().getVideoUrl()) : "";
}

QString serio::qt::TvShowPlayerViewModel::getTvShowName() const {
    return tvShowName;
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
    tvShowName = args[0].toString();
    player.reset();
    if (tvShowPlayer.isTvShowWatchComplete(tvShowName.toStdString())) {
        displayTvShowIsOverDialog();
    } else {
        play(tvShowPlayer.playTvShow(args[0].toString().toStdString()));
        stack.pushView(tvShowPlayerView);
    }
}

void serio::qt::TvShowPlayerViewModel::play(serio::core::Player &&newPlayer) {
    player = newPlayer;
    tvShowName = QString::fromStdString(player->getPlayingTvShowName());
    emit playingEpisodeChanged();
}

double serio::qt::TvShowPlayerViewModel::getOffsetPercentage() const {
    return player ? player->getStartPercentage() : 0;
}

bool serio::qt::TvShowPlayerViewModel::hasPreviousEpisode() const {
    return player && player->hasPreviousEpisode();
}

bool serio::qt::TvShowPlayerViewModel::hasNextEpisode() const {
    return player && player->hasNextEpisode();
}

void serio::qt::TvShowPlayerViewModel::playPreviousEpisode() {
    play(tvShowPlayer.playPreviousEpisode());
}

void serio::qt::TvShowPlayerViewModel::playNextEpisode() {
    if (hasNextEpisode()) {
        play(tvShowPlayer.playNextEpisode());
    } else {
        displayTvShowIsOverDialog();
    }
}

QString serio::qt::TvShowPlayerViewModel::formatDuration(unsigned int duration) const {
    std::chrono::milliseconds durationMs(duration);
    std::chrono::minutes minutes = std::chrono::duration_cast<std::chrono::minutes>(durationMs);
    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(durationMs - minutes);
    return QString::number(minutes.count()) + ":" + QString::number(seconds.count()).rightJustified(2, QLatin1Char('0'));
}

void serio::qt::TvShowPlayerViewModel::displayTvShowIsOverDialog() {
    serio::qt::DialogModel model("'" + tvShowName + "' is over",
                                 "You have watched all available episodes of '" + tvShowName + "'.");
    model.setTopButtonAction(serio::qt::ActionType::RE_WATCH_CURRENT_TV_SHOW);
    model.setTopButtonText("rewatch");
    model.setBottomButtonAction(serio::qt::ActionType::GO_TO_ALL_TV_SHOWS);
    model.setBottomButtonText("watch another");
    dialog.display(model);
}

void serio::qt::TvShowPlayerViewModel::rewatchCurrentTvShow() {
    bool isCurrentlyPlaying = player.has_value();
    play(tvShowPlayer.playEpisodeOfTvShow(tvShowName.toStdString(), 1));
    if (isCurrentlyPlaying) {
        stack.popCurrentView();
    } else {
        stack.replaceCurrentViewWith(tvShowPlayerView);
    }
}
