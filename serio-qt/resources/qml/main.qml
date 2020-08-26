import QtQuick 2.12
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
    Material.theme: Material.Dark
    Material.accent: Material.Orange
    property real cardPadding: 16
    HorizontalCardList {
        id: allShowsList
        anchors.fill: parent
        anchors.margins: cardPadding
        cardSpacing: cardPadding
        model: allTvShowsViewModel.allShowsList
        focus: true
        delegate: TvShowCard {
            height: allShowsList.height
            tvShowName: model.name
            thumbnailUrl: model.thumbnailUrl
            lastWatchDate: model.lastWatchDate || null
            thumbnailSourceHeight: Screen.desktopAvailableHeight * Screen.devicePixelRatio
        }
    }
}
