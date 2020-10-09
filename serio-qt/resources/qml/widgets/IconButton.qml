import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "../../lib/qml-material-icons/src/MaterialDesign.js" as MD

RoundButton {
    property string iconName
    font.family: materialFont.name
    font.pixelSize: 28
    text: MD.icons[iconName]
    highlighted: activeFocus
    Material.foreground: Material.accent
    Keys.onReturnPressed: clicked()
    Keys.onEnterPressed: clicked()
    FontLoader {
        id: materialFont
        source: "qrc:/lib/qml-material-icons/fonts/MaterialIcons-Regular.ttf"
    }
}
