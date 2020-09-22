import QtQuick 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

ViewLayout {
    onCreated: {
        backBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
        viewLogBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_PREVIEWED_CRAWLER_LOG, [crawlerEditorViewModel.crawlerType]))
    }
    AccentTitle {
        text: crawlerEditorViewModel.crawlerType + " Crawler Preview"
    }
    Row {
        Layout.fillWidth: true
        spacing: globalPadding
        SerioButton {
            id: backBtn
            text: "back"
            focus: true
            KeyNavigation.tab: viewLogBtn
            KeyNavigation.right: viewLogBtn
            KeyNavigation.down: previewResultsList
        }
        SerioButton {
            id: viewLogBtn
            text: "view log"
            KeyNavigation.tab: previewResultsList
            KeyNavigation.down: previewResultsList
        }
    }
    VerticalTileList {
        id: previewResultsList
        Layout.fillHeight: true
        Layout.fillWidth: true
        model: crawlerEditorViewModel.previewResults
        tilePadding: globalPadding / 2
        KeyNavigation.tab: backBtn
    }
}
