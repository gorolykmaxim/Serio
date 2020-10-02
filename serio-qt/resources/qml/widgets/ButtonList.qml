import QtQuick 2.12
import QtQuick.Layouts 1.12

ListView {
    height: currentItem !== null ? currentItem.height : 0
    orientation: ListView.Horizontal
    delegate: SerioButton {
        text: model.modelData.text
        onClicked: actionRouter.trigger(model.modelData.clickAction, model.modelData.clickArguments)
    }
}