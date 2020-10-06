import QtQuick 2.0

MouseArea {
    property bool cursorVisible: true
    hoverEnabled: true
    cursorShape: !cursorVisible ? Qt.BlankCursor : Qt.ArrowCursor
}
