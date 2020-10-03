import QtQuick 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

CenteredViewLayout {
    onCreated: {
        editBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_CURRENT_TV_SHOW_CRAWLER_EDITOR_VIEW, []))
        shareBtn.clicked.connect(() => actionRouter.trigger(ActionType.SHARE_CRAWLER_OF_CURRENT_TV_SHOW, []))
        crawlBtn.clicked.connect(() => actionRouter.trigger(ActionType.CRAWL_CURRENT_TV_SHOW, []))
        viewCrawlLogBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_LAST_TV_SHOW_CRAWL_LOG, [tvShowViewModel.tvShowName]))
        clearWatchHistoryBtn.clicked.connect(() => actionRouter.trigger(ActionType.CONFIRM_CLEAR_CURRENT_TV_SHOW_WATCH_HISTORY, []))
        deleteBtn.clicked.connect(() => actionRouter.trigger(ActionType.CONFIRM_DELETE_CURRENT_TV_SHOW, []))
        backBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
    }
    onDisplayed: actionRouter.trigger(ActionType.LOAD_TV_SHOW, [])
    titleText: tvShowViewModel.tvShowName
    subtitleText: tvShowViewModel.lastWatchDate
    RaisedButton {
        id: editBtn
        text: "edit"
        Layout.fillWidth: true
        KeyNavigation.down: shareBtn
        KeyNavigation.tab: shareBtn
    }
    RaisedButton {
        id: shareBtn
        text: "share crawler"
        Layout.fillWidth: true
        KeyNavigation.down: crawlBtn
        KeyNavigation.tab: crawlBtn
    }
    RaisedButton {
        id: crawlBtn
        text: "crawl"
        Layout.fillWidth: true
        KeyNavigation.down: viewCrawlLogBtn
        KeyNavigation.tab: viewCrawlLogBtn
    }
    RaisedButton {
        id: viewCrawlLogBtn
        text: "view crawl log"
        Layout.fillWidth: true
        KeyNavigation.down: clearWatchHistoryBtn
        KeyNavigation.tab: clearWatchHistoryBtn
    }
    RaisedButton {
        id: clearWatchHistoryBtn
        text: "clear watch history"
        Layout.fillWidth: true
        KeyNavigation.down: deleteBtn
        KeyNavigation.tab: deleteBtn
    }
    RaisedButton {
        id: deleteBtn
        text: "delete"
        Layout.fillWidth: true
        KeyNavigation.down: backBtn
        KeyNavigation.tab: backBtn
    }
    RaisedButton {
        id: backBtn
        text: "back"
        primary: false
        Layout.fillWidth: true
        focus: true
        KeyNavigation.tab: editBtn
    }
}
