import QtQuick 2.12
import QtQuick.Layouts 1.12

Item {
    id: root
    property string titleText
    property string subtitleText
    property string position
    property string duration
    property real progress
    property real padding
    property bool isPlaying
    signal backPressed()
    signal playPressed()
    signal pausePressed()
    signal forwardPressed()
    signal replayPressed()
    signal userInteraction()
    TopBottomShade {
        display: true
        anchors.fill: parent
    }
    LeftAnchoredRow {
        anchors.top: parent.top
        spacing: padding
        IconButton {
            id: backBtn
            iconName: "keyboard_backspace"
            onClicked: {
                userInteraction()
                backPressed()
            }
            onGotFocus: userInteraction()
            KeyNavigation.tab: playPauseBtn
            KeyNavigation.down: playPauseBtn
        }
        AccentTitleAndSubtitle {
            title: titleText
            subtitle: subtitleText
        }
    }
    VideoProgressBar {
        anchors.bottom: controlsRow.top
        spacing: padding
        position: parent.position
        duration: parent.duration
        progress: parent.progress
    }
    LeftAnchoredRow {
        id: controlsRow
        anchors.bottom: parent.bottom
        spacing: padding
        IconButton {
            id: playPauseBtn
            focus: true
            iconName: isPlaying ? "pause" : "play_arrow"
            onClicked: {
                userInteraction()
                if (isPlaying)
                    pausePressed()
                else
                    playPressed()
            }
            onGotFocus: userInteraction()
            KeyNavigation.right: replayBtn
            KeyNavigation.tab: replayBtn
        }
        IconButton {
            id: replayBtn
            iconName: "replay_30"
            onClicked: {
                userInteraction()
                replayPressed()
            }
            onGotFocus: userInteraction()
            KeyNavigation.up: backBtn
            KeyNavigation.right: forwardBtn
            KeyNavigation.tab: forwardBtn
        }
        IconButton {
            id: forwardBtn
            iconName: "forward_30"
            onClicked: {
                userInteraction()
                forwardPressed()
            }
            onGotFocus: userInteraction()
            KeyNavigation.up: backBtn
            KeyNavigation.tab: backBtn
        }
    }
    PassthroughCursorArea {
        anchors.fill: parent
        cursorVisible: root.opacity > 0
        onMouseMoved: userInteraction()
    }
    TapHandler {
        onTapped: userInteraction()
    }
    Keys.onPressed: userInteraction()
}
