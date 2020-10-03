import QtQuick 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

ViewLayout {
    titleText: crawlerEditorViewModel.crawlerType + " Preview"
    ButtonList {
        id: actionsList
        Layout.fillWidth: true
        spacing: globalPadding
        focus: true
        model: crawlerEditorViewModel.crawlerPreviewActions
        KeyNavigation.down: previewResultsList
        KeyNavigation.tab: previewResultsList
    }
    VerticalTileList {
        id: previewResultsList
        Layout.fillHeight: true
        Layout.fillWidth: true
        model: crawlerEditorViewModel.previewResults
        tilePadding: globalPadding / 2
        KeyNavigation.tab: actionsList
    }
}
