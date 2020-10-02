import QtQuick 2.0

Flickable {
    default property alias contents: content.children
    property alias spacing: content.spacing
    contentHeight: content.height
    clip: true
    Column {
        id: content
        width: parent.width
    }
}
