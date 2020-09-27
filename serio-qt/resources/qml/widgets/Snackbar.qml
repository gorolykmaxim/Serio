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
    text: snackbarViewModel.text
    anchors {
        horizontalCenter: parent.horizontalCenter
        bottom: parent.bottom
        bottomMargin: snackbarViewModel.displayed ? outerPadding : -height
        Behavior on bottomMargin {
            NumberAnimation {
                duration: 400
                easing.type: Easing.InBack
            }
        }
    }
    Timer {
        id: stopTimer
        interval: 2000
        onTriggered: snackbarViewModel.hide()
    }
}
