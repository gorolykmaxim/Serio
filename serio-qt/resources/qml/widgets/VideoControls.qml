import QtQuick 2.12
import QtQuick.Layouts 1.12

Item {
    id: root
    property string titleText
    property string subtitleText
    property real padding
    signal backPressed()
    signal userInteraction()
    TopShade {
        display: true
        anchors.fill: parent
    }
    RowLayout {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: padding
        spacing: padding
        IconButton {
            focus: true
            iconName: "keyboard-backspace"
            onClicked: {
                userInteraction()
                backPressed()
            }
        }
        AccentTitleAndSubtitle {
            title: titleText
            subtitle: subtitleText
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
