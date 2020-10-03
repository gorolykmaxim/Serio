import QtQuick 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

View {
    onCreated: {
        backBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
        editBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_CURRENT_TV_SHOW_CRAWLER_EDITOR_VIEW, []))
        shareBtn.clicked.connect(() => actionRouter.trigger(ActionType.SHARE_CRAWLER_OF_CURRENT_TV_SHOW, []))
        crawlBtn.clicked.connect(() => actionRouter.trigger(ActionType.CRAWL_CURRENT_TV_SHOW, []))
        viewCrawlLogBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_LAST_TV_SHOW_CRAWL_LOG, [tvShowViewModel.tvShowName]))
        clearWatchHistoryBtn.clicked.connect(() => actionRouter.trigger(ActionType.CONFIRM_CLEAR_CURRENT_TV_SHOW_WATCH_HISTORY, []))
        deleteBtn.clicked.connect(() => actionRouter.trigger(ActionType.CONFIRM_DELETE_CURRENT_TV_SHOW, []))
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
                KeyNavigation.right: editBtn
                KeyNavigation.tab: editBtn
            }
            RaisedButton {
                id: editBtn
                text: "edit"
                focus: episodeList.count === 0
                KeyNavigation.down: episodeList
                KeyNavigation.right: shareBtn
                KeyNavigation.tab: shareBtn
            }
            RaisedButton {
                id: shareBtn
                text: "share crawler"
                KeyNavigation.down: episodeList
                KeyNavigation.right: crawlBtn
                KeyNavigation.tab: crawlBtn
            }
            RaisedButton {
                id: crawlBtn
                text: "crawl"
                KeyNavigation.down: episodeList
                KeyNavigation.right: viewCrawlLogBtn
                KeyNavigation.tab: viewCrawlLogBtn
            }
            RaisedButton {
                id: viewCrawlLogBtn
                text: "view crawl log"
                KeyNavigation.down: episodeList
                KeyNavigation.right: clearWatchHistoryBtn
                KeyNavigation.tab: clearWatchHistoryBtn
            }
            RaisedButton {
                id: clearWatchHistoryBtn
                text: "clear watch history"
                KeyNavigation.down: episodeList
                KeyNavigation.right: deleteBtn
                KeyNavigation.tab: deleteBtn
            }
            RaisedButton {
                id: deleteBtn
                text: "delete"
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
