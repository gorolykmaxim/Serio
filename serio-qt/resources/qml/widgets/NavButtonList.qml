import QtQuick 2.12

ListView {
    height: currentItem !== null ? currentItem.height : 0
    orientation: ListView.Horizontal
    delegate: NavButton {
        text: model.modelData.text
        highlighted: model.modelData.highlighted
        onClicked: actionRouter.trigger(model.modelData.clickAction, [])
    }
}
