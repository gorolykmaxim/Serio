import QtQuick 2.12
import QtQuick.Layouts 1.12

Item {
    id: root
    property string titleText
    property string subtitleText
    property real padding
    property bool isPlaying
    signal backPressed()
    signal playPressed()
    signal pausePressed()
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
            iconName: "keyboard-backspace"
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
    LeftAnchoredRow {
        anchors.bottom: parent.bottom
        spacing: padding
        IconButton {
            id: playPauseBtn
            focus: true
            iconName: isPlaying ? "pause" : "play"
            onClicked: {
                userInteraction()
                if (isPlaying)
                    pausePressed()
                else
                    playPressed()
            }
            onGotFocus: userInteraction()
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
