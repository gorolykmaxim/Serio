import QtQuick 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

View {
    onCreated: {
        backBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
        editBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_CURRENT_TV_SHOW_CRAWLER_EDITOR_VIEW, []))
        shareBtn.clicked.connect(() => actionRouter.trigger(ActionType.SHARE_CRAWLER_OF_CURRENT_TV_SHOW, []))
    }
    onDisplayed: actionRouter.trigger(ActionType.LOAD_TV_SHOW, [])
    RoundNullableImage {
        anchors.fill: parent
        imageUrl: tvShowViewModel.thumbnailUrl
        borderRadius: 0
        opacity: 0.2
    }
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
            SerioButton {
                id: backBtn
                text: "back"
                focus: episodeList.count > 0
                KeyNavigation.down: episodeList
                KeyNavigation.right: editBtn
                KeyNavigation.tab: editBtn
            }
            SerioButton {
                id: editBtn
                text: "edit"
                focus: episodeList.count === 0
                KeyNavigation.down: episodeList
                KeyNavigation.right: shareBtn
                KeyNavigation.tab: shareBtn
            }
            SerioButton {
                id: shareBtn
                text: "share crawler"
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
