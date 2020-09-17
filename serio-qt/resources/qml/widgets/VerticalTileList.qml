import QtQuick 2.0

AnimatedList {
    id: root
    property real tilePadding
    signal itemClicked(int index)
    clip: true
    orientation: ListView.Vertical
    delegate: ListTile {
        title: modelData.title
        subtitle: modelData.subtitle
        tilePadding: root.tilePadding
        highlighted: root.activeFocus && ListView.isCurrentItem
        onClicked: itemClicked(index)
    }
}
