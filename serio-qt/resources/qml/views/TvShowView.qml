import QtQuick 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

ViewLayout {
    onCreated: episodeList.itemClicked.connect((i) => actionRouter.trigger(ActionType.PLAY_EPISODE_OF_TV_SHOW, [tvShowViewModel.tvShowName, i + 1]))
    onDisplayed: actionRouter.trigger(ActionType.LOAD_TV_SHOW, [])
    titleText: tvShowViewModel.tvShowName
    subtitleText: tvShowViewModel.lastWatchDate
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
        visible: count > 0
        tilePadding: globalPadding / 2
        KeyNavigation.tab: actionsList
    }
    NoContentPlaceholder {
        contentType: "episodes"
        Layout.fillWidth: true
        Layout.fillHeight: true
        visible: episodeList.count === 0
    }
}
