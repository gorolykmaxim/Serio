import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0
import "../animations"

FocusScope {
    id: root
    width: 300
    height: 300
    property string thumbnailUrl
    property string tvShowName
    property string lastWatchDate
    property real borderRadius: 4
    signal selected()
    Keys.onReturnPressed: selected()
    Keys.onEnterPressed: selected()
    RoundNullableImage {
        anchors.fill: parent
        borderRadius: parent.borderRadius
        imageUrl: thumbnailUrl
    }
    BottomShade {
        anchors.fill: parent
        display: root.activeFocus || cursorArea.containsMouse
        radius: borderRadius
        Behavior on opacity {
            SerioNumberAnimation {}
        }
    }
    BottomColumnLayout {
        height: root.activeFocus || cursorArea.containsMouse ? implicitHeight : 0
        Title {
            text: tvShowName
            Layout.preferredWidth: parent.width
        }
        Subtitle {
            text: lastWatchDate
            Layout.preferredWidth: parent.width
            visible: lastWatchDate
        }
        Behavior on height {
            SerioNumberAnimation {}
        }
    }
    CursorArea {
        id: cursorArea
        anchors.fill: parent
        onClicked: selected()
    }
}
