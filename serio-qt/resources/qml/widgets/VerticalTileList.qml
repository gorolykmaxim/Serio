import QtQuick 2.0

AnimatedList {
    id: root
    property real tilePadding
    signal itemClicked(int index)
    clip: true
    orientation: ListView.Vertical
    delegate: ListTile {
        title: model.modelData ? model.modelData.title : model.title
        subtitle: model.modelData ? model.modelData.subtitle : model.subtitle
        tileIcon: model.modelData ? model.modelData.icon : model.icon
        width: root.width
        tilePadding: root.tilePadding
        highlighted: root.activeFocus && ListView.isCurrentItem
        onClicked: itemClicked(index)
    }
}
