import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "widgets"

ApplicationWindow {
    Component.onCompleted: {
        allTvShowsViewModel.reload()
    }
    visible: true
    width: 800
    height: 600
    minimumWidth: 800
    title: qsTr("Serio")
    property real cardPadding: 16
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: cardPadding
        spacing: cardPadding
        AccentTitle {
            text: "Last Watched"
            visible: watchedShowsList.visible
        }
        TvShowCardList {
            id: watchedShowsList
            Layout.fillHeight: true
            Layout.fillWidth: true
            cardSpacing: cardPadding
            focus: true
            visible: count > 0
            listModel: allTvShowsViewModel.watchedShowsList
        }
        TvShowCardList {
            Layout.fillHeight: true
            Layout.fillWidth: true
            cardSpacing: cardPadding
            listModel: allTvShowsViewModel.allShowsList
        }
    }
}
