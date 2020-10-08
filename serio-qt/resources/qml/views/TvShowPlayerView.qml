import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtMultimedia 5.12
import Serio 1.0
import '../widgets'
import '../animations'

View {
    onCreated: {
        controls.onBackPressed.connect(() => actionRouter.trigger(ActionType.BACK, []))
        controls.onUserInteraction.connect(displayControls)
        controls.onPlayPressed.connect(() => video.play())
        controls.onPausePressed.connect(() => video.pause())
        controls.onForwardPressed.connect(() => video.seek(video.position + 30000))
        controls.onReplayPressed.connect(() => video.seek(video.position - 30000))
        watchProgressSavingTimer.onTriggered.connect(() => actionRouter.trigger(ActionType.SET_PLAYING_EPISODE_PROGRESS, [video.position, video.duration]))
        video.onSeekableChanged.connect(() => video.seek(video.duration * tvShowPlayerViewModel.offsetPercentage / 100))
    }
    onDisplayed: displayControls()
    function displayControls() {
        controls.opacity = 1
        hideControlsTimer.restart()
    }
    function hideControlsIfPlaying() {
        if (video.playbackState === MediaPlayer.PlayingState)
            controls.opacity = 0
    }
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
}
