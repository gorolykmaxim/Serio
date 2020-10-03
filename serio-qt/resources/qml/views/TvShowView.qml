import QtQuick 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

View {
    onCreated: {
        backBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
        detailsBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_TV_SHOW_DETAILS_VIEW, []))
    }
    onDisplayed: actionRouter.trigger(ActionType.LOAD_TV_SHOW, [])
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: globalPadding
        spacing: globalPadding
        AccentTitleAndSubtitle{
            title: tvShowViewModel.tvShowName
            subtitle: tvShowViewModel.lastWatchDate
        }
        Row {
            Layout.fillWidth: true
            spacing: globalPadding
            RaisedButton {
                id: backBtn
                text: "back"
                focus: episodeList.count > 0
                KeyNavigation.down: episodeList
                KeyNavigation.right: detailsBtn
                KeyNavigation.tab: detailsBtn
            }
            RaisedButton {
                id: detailsBtn
                text: "details"
                focus: episodeList.count == 0
                KeyNavigation.down: episodeList
                KeyNavigation.tab: episodeList
            }
        }
        VerticalTileList {
            id: episodeList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: tvShowViewModel.episodeList
            tilePadding: globalPadding / 2
            KeyNavigation.tab: backBtn
        }
    }
}
