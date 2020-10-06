import QtQuick 2.12

CursorArea {
    signal mouseMoved()
    onPositionChanged: {
        mouseMoved()
        mouse.accepted = false
    }
    onClicked: mouse.accepted = false
    onDoubleClicked: mouse.accepted = false
    onPressAndHold: mouse.accepted = false
    onPressed: mouse.accepted = false
    onReleased: mouse.accepted = false
}
