import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

ViewLayout {
    onCreated: textArea.editingFinished.connect(() => actionRouter.trigger(ActionType.SET_RAW_TV_SHOW_CRAWLER_TO_IMPORT, [textArea.text]))
    AccentTitle {
        text: "Import TV Show Crawler"
    }
    ButtonList {
        id: actionsList
        Layout.fillWidth: true
        spacing: globalPadding
        focus: true
        model: tvShowCrawlerEditorViewModel.importTvShowCrawlerActions
        KeyNavigation.down: textArea
        KeyNavigation.tab: textArea
    }
    Flickable {
        Layout.fillWidth: true
        Layout.fillHeight: true
        TextArea.flickable: TextArea {
            id: textArea
            focus: true
            placeholderText: "Paste configuration of a tv show crawler here"
            KeyNavigation.priority: KeyNavigation.BeforeItem
            KeyNavigation.tab: actionsList
        }
        ScrollBar.vertical: ScrollBar {}
    }
}
