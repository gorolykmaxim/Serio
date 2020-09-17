import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Page {
    property real globalPadding: 16
    signal created()
    signal displayed()
    Component.onCompleted: created()
    StackView.onStatusChanged: {
        if (StackView.status == StackView.Activating) {
            displayed()
        }
    }
}
