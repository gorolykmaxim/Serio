import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Button {
    property bool primary: true
    highlighted: activeFocus
    flat: !primary
    font.capitalization: Font.AllUppercase
    Material.foreground: Material.accent
    Keys.onReturnPressed: clicked()
    Keys.onEnterPressed: clicked()
}
