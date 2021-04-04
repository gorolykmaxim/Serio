import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Serio 1.0

Page {
    property real globalPadding: 16
    property string viewUrl
    signal created()
    signal displayed()
    background: null
    Component.onCompleted: created()
    StackView.onStatusChanged: {
        if (StackView.status == StackView.Activating) {
            displayed()
        }
    }
    Keys.onBackPressed: {
        if (viewStack.depth > 1) {
            event.accepted = true
            actionRouter.trigger(ActionType.BACK, [])
        } else {
            event.accepted = false
        }
    }
}
