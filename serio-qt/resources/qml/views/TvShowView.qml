import QtQuick 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

View {
    onDisplayed: actionRouter.trigger(ActionType.LOAD_TV_SHOW, [])
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: globalPadding
        spacing: globalPadding
        AccentTitleAndSubtitle{
            title: tvShowViewModel.tvShowName
            subtitle: tvShowViewModel.lastWatchDate
        }
        ButtonList {
            id: actionsList
            Layout.fillWidth: true
            spacing: globalPadding
            focus: true
            model: tvShowViewModel.actions
            KeyNavigation.down: episodeList
            KeyNavigation.tab: episodeList
        }
        VerticalTileList {
            id: episodeList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: tvShowViewModel.episodeList
            tilePadding: globalPadding / 2
            KeyNavigation.tab: actionsList
        }
    }
}
