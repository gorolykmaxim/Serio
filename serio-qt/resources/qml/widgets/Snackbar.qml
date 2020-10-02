import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import '../animations'

Paragraph {
    property real outerPadding: 16
    Connections {
        target: snackbarViewModel
        function onHasBeenDisplayed() { stopTimer.start() }
    }
    background: Rectangle {
        color: Material.accent
        radius: 4
    }
    topPadding: outerPadding - 4
    bottomPadding: outerPadding - 4
    leftPadding: outerPadding
    rightPadding: outerPadding
    width: 300
    horizontalAlignment: Text.AlignHCenter
    text: snackbarViewModel.text
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    anchors.bottomMargin: outerPadding
    opacity: snackbarViewModel.displayed ? 1 : 0
    Behavior on opacity {
        SerioNumberAnimation {}
    }
    Timer {
        id: stopTimer
        interval: 2000
        onTriggered: snackbarViewModel.hide()
    }
}
