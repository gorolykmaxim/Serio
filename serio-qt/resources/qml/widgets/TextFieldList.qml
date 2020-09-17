import QtQuick 2.12
import QtQuick.Controls 2.12

ListView {
    clip: true
    keyNavigationEnabled: true
    delegate: TextField {
        width: parent.width
        placeholderText: modelData.name
        text: modelData.value
        onTextEdited: modelData.value = text
        activeFocusOnTab: false
    }
}
