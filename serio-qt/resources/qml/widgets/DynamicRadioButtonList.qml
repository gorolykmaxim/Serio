import QtQuick 2.12
import '../widgets'

ListView {
    id: root
    keyNavigationEnabled: true
    orientation: ListView.Horizontal
    signal itemSelected(string name)
    delegate: DynamicRadioButton {
        onClicked: itemSelected(modelData.name)
        buttonName: modelData.name
        buttonChecked: modelData.checked
        highlighted: root.activeFocus && ListView.isCurrentItem
    }
}
