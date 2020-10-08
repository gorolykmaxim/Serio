import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

RowLayout {
    property string position
    property string duration
    property real progress
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: spacing
    Subtitle {
        text: position
    }
    ProgressBar {
        Layout.fillWidth: true
        value: progress
    }
    Subtitle {
        text: duration
    }
}
