import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

ViewLayout {
    onCreated: {
        crawlerStepList.itemClicked.connect((index) => actionRouter.trigger(ActionType.OPEN_EXISTING_CRAWLER_STEP_EDITOR, [index]))
    }
    onDisplayed: actionRouter.trigger(ActionType.LOAD_CRAWLER_STEPS, [])
    AccentTitle {
        text: crawlerEditorViewModel.crawlerType + " Crawler"
    }
    ButtonList {
        id: actionsList
        Layout.fillWidth: true
        spacing: globalPadding
        focus: true
        model: crawlerEditorViewModel.crawlerEditorActions
        KeyNavigation.down: crawlerStepList
        KeyNavigation.tab: crawlerStepList
    }
    VerticalTileList {
        id: crawlerStepList
        Layout.fillHeight: true
        Layout.fillWidth: true
        model: crawlerEditorViewModel.crawlerSteps
        tilePadding: globalPadding / 2
        KeyNavigation.tab: actionsList
    }
}
