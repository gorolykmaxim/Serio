import QtQuick 2.12
import QtQuick.Layouts 1.12

ListView {
    height: currentItem !== null ? currentItem.height : 0
    orientation: ListView.Horizontal
    delegate: RaisedButton {
        text: model.modelData.text
        primary: model.modelData.primary
        onClicked: actionRouter.trigger(model.modelData.clickAction, model.modelData.clickArguments)
    }
}
