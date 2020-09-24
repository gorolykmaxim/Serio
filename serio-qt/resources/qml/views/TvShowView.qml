import QtQuick 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

View {
    onCreated: {
        backBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
        tvShowViewModel.episodeList.requestPageLoad.connect((offset, limit) => actionRouter.trigger(ActionType.LOAD_EPISODES_LIST_PAGE, [offset, limit]))
    }
    onDisplayed: actionRouter.trigger(ActionType.LOAD_TV_SHOW, [])
    RoundNullableImage {
        anchors.fill: parent
        imageUrl: tvShowViewModel.thumbnailUrl
        borderRadius: 0
    }
    Shade {
        anchors.fill: parent
    }
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: globalPadding
        spacing: globalPadding
        AccentTitleAndSubtitle{
            title: tvShowViewModel.tvShowName
            subtitle: tvShowViewModel.lastWatchDate
        }
        SerioButton {
            id: backBtn
            text: "back"
            focus: true
            KeyNavigation.down: episodeList
            KeyNavigation.tab: episodeList
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
