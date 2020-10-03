import QtQuick 2.12
import QtQuick.Controls 2.12
import 'widgets'

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("Serio")
    Background {
        anchors.fill: parent
    }
    StackOfViews {
        id: viewStack
        initialItem: "views/AllTvShowsView.qml"
    }
    Snackbar {}
}
