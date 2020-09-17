import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

View {
    property real layoutSpacing: 0
    default property alias contents: placeholder.children
    ColumnLayout {
        id: placeholder
        spacing: layoutSpacing
        anchors.centerIn: parent
    }
}
