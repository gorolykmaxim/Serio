import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtMultimedia 5.12
import Serio 1.0
import '../widgets'
import '../animations'

View {
    property bool hasSeekedInitialOffset: false
    onCreated: {
        controls.onBackPressed.connect(() => actionRouter.trigger(ActionType.BACK, []))
        controls.onUserInteraction.connect(displayControls)
        controls.onPlayPressed.connect(() => video.play())
        controls.onPausePressed.connect(() => video.pause())
        controls.onForwardPressed.connect(() => video.seek(video.position + 30000))
        controls.onReplayPressed.connect(() => video.seek(video.position - 30000))
        controls.onPlayPreviousEpisodePressed.connect(() => actionRouter.trigger(ActionType.PLAY_PREVIOUS_EPISODE, []))
        controls.onPlayNextEpisodePressed.connect(() => actionRouter.trigger(ActionType.PLAY_NEXT_EPISODE, []))
        watchProgressSavingTimer.onTriggered.connect(() => actionRouter.trigger(ActionType.SET_PLAYING_EPISODE_PROGRESS, [video.position, video.duration]))
        video.onStatusChanged.connect(onVideoStatusChange)
        video.onErrorChanged.connect(() => actionRouter.trigger(ActionType.DISPLAY_ERROR, ["Failed to play " + tvShowPlayerViewModel.episodeName + " of " + tvShowPlayerViewModel.tvShowName + ": " + (video.errorString || "Unknown error"), true]))
    }
    onDisplayed: displayControls()
    Timer {
        id: watchProgressSavingTimer
        interval: 5000
        repeat: true
        running: video.playbackState === MediaPlayer.PlayingState && video.seekable
    }
    Timer {
        id: hideControlsTimer
        interval: 5000
        onTriggered: hideControlsIfPlaying()
    }
    Video {
        id: video
        anchors.fill: parent
        autoPlay: true
        source: tvShowPlayerViewModel.episodeVideoUrl
    }
    VideoControls {
        id: controls
        anchors.fill: parent
        titleText: tvShowPlayerViewModel.episodeName
        subtitleText: tvShowPlayerViewModel.tvShowName
        isPlaying: video.playbackState === MediaPlayer.PlayingState
        position: tvShowPlayerViewModel.formatDuration(video.position)
        duration: tvShowPlayerViewModel.formatDuration(video.duration)
        progress: video.position / video.duration
        hasPreviousEpisode: tvShowPlayerViewModel.hasPreviousEpisode
        hasNextEpisode: tvShowPlayerViewModel.hasNextEpisode
        padding: globalPadding
        opacity: 0
        Behavior on opacity {
            FastNumberAnimation {}
        }
    }
    NonDeterministicCircularProgressBar {
        anchors.centerIn: parent
        visible: video.status === MediaPlayer.Buffering || video.status === MediaPlayer.Loading
    }
    function displayControls() {
        controls.opacity = 1
        hideControlsTimer.restart()
    }
    function hideControlsIfPlaying() {
        if (video.playbackState === MediaPlayer.PlayingState)
            controls.opacity = 0
    }
    function onVideoStatusChange() {
        if (video.status === MediaPlayer.EndOfMedia) {
            actionRouter.trigger(ActionType.PLAY_NEXT_EPISODE, [])
        } else if (video.status === MediaPlayer.Buffered && !hasSeekedInitialOffset) {
            video.seek(video.duration * tvShowPlayerViewModel.offsetPercentage / 100)
            hasSeekedInitialOffset = true;
        }
    }
}
