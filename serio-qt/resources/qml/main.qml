import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    visible: true
    width: 800
    height: 600
    minimumWidth: 800
    color: "black"
    title: qsTr("Serio")
    Label {
        text: helloWorld
        color: "orange"
    }
}
