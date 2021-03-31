import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    flat: true
    Keys.onReturnPressed: clicked()
    Keys.onEnterPressed: clicked()
}
