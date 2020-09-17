import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

View {
    default property alias contents: placeholder.children
    ColumnLayout {
        id: placeholder
        anchors.fill: parent
        anchors.margins: globalPadding
        spacing: globalPadding
    }
}
