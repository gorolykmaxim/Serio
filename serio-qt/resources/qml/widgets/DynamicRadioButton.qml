import QtQuick 2.12
import QtQuick.Controls 2.12

RadioDelegate {
    property string buttonName
    property bool buttonChecked: false
    text: buttonName
    checked: buttonChecked
    font.capitalization: Font.Capitalize
    Keys.onEnterPressed: clicked()
    Keys.onReturnPressed: clicked()
}
