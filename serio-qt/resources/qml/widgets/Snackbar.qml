import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import '../animations'

Subtitle {
    property real outerPadding
    Connections {
        target: snackbarViewModel
        function onHasBeenDisplayed() { stopTimer.start() }
    }
    background: Rectangle {
        color: Material.accent
        radius: 4
    }
    topPadding: outerPadding / 1.6
    bottomPadding: outerPadding / 1.6
    leftPadding: outerPadding * 2.4
    rightPadding: outerPadding * 2.4
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    anchors.bottomMargin: outerPadding
    opacity: snackbarViewModel.displayed ? 1 : 0
    text: snackbarViewModel.text
    Behavior on opacity {
        SerioNumberAnimation {}
    }
    Timer {
        id: stopTimer
        interval: 2000
        onTriggered: snackbarViewModel.hide()
    }
}
