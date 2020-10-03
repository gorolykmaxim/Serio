import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import '../widgets'

View {
    property real layoutSpacing: 0
    property string titleText: ""
    property string subtitleText: ""
    default property alias contents: placeholder.children
    AccentTitleAndSubtitle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: globalPadding
        title: titleText
        subtitle: subtitleText
    }
    ColumnLayout {
        id: placeholder
        spacing: layoutSpacing
        width: 300 - globalPadding
        anchors.centerIn: parent
    }
}
