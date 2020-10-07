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
        watchProgressSavingTimer.onTriggered.connect(() => actionRouter.trigger(ActionType.SET_PLAYING_EPISODE_PROGRESS, [video.position, video.duration]))
    }
    onDisplayed: displayControls()
    function displayControls() {
        controls.opacity = 1
        hideControlsTimer.restart()
    }
    Timer {
        id: watchProgressSavingTimer
        interval: 5000
        repeat: true
        running: video.playbackState === MediaPlayer.PlayingState
    }
    Timer {
        id: hideControlsTimer
        interval: 5000
        onTriggered: controls.opacity = 0
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
