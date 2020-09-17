import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    minimumWidth: 800
    title: qsTr("Serio")
    StackOfViews {
        id: viewStack
        initialItem: "views/AllShowsView.qml"
    }
}
