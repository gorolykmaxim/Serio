import QtQuick 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

ViewLayout {
    onCreated: {
        backBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
        logList.itemClicked.connect((index) => actionRouter.trigger(ActionType.OPEN_CRAWL_LOG_ENTRY_VIEW, [index]))
    }
    titleText: crawlLogViewModel.title
    RaisedButton {
        id: backBtn
        text: "back"
        focus: true
        primary: false
        KeyNavigation.tab: logList
        KeyNavigation.down: logList
    }
    VerticalTileList {
        id: logList
        Layout.fillWidth: true
        Layout.fillHeight: true
        model: crawlLogViewModel.log
        tilePadding: globalPadding / 2
        KeyNavigation.tab: backBtn
    }
}
