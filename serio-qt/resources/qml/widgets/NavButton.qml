import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    flat: !activeFocus
    Keys.onReturnPressed: clicked()
    Keys.onEnterPressed: clicked()
}
