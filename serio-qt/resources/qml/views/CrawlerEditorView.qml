import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

ViewLayout {
    onCreated: {
        cancelBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
        addStepBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_NEW_CRAWLER_STEP_EDITOR, []))
        saveBtn.clicked.connect(() => actionRouter.trigger(ActionType.SAVE_CRAWLER, []))
        helpBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_CRAWLER_EDITOR_HELP, []))
        crawlerStepList.itemClicked.connect((index) => actionRouter.trigger(ActionType.OPEN_EXISTING_CRAWLER_STEP_EDITOR, [index]))
    }
    onDisplayed: actionRouter.trigger(ActionType.LOAD_CRAWLER_STEPS, [])
    AccentTitle {
        text: crawlerEditorViewModel.crawlerType + " Crawler Steps"
    }
    Row {
        Layout.fillWidth: true
        spacing: globalPadding
        SerioButton {
            id: cancelBtn
            text: "cancel"
            KeyNavigation.right: helpBtn
            KeyNavigation.tab: helpBtn
        }
        SerioButton {
            id: helpBtn
            text: "help"
            KeyNavigation.right: addStepBtn
            KeyNavigation.tab: addStepBtn
            KeyNavigation.down: crawlerStepList
        }
        SerioButton {
            id: addStepBtn
            focus: true
            text: "add step"
            KeyNavigation.right: saveBtn
            KeyNavigation.tab: saveBtn
            KeyNavigation.down: crawlerStepList
        }
        SerioButton {
            id: saveBtn
            text: "save"
            KeyNavigation.tab: crawlerStepList
            KeyNavigation.down: crawlerStepList
        }
    }
    VerticalTileList {
        id: crawlerStepList
        Layout.fillHeight: true
        Layout.fillWidth: true
        model: crawlerEditorViewModel.crawlerSteps
        tilePadding: globalPadding / 2
        KeyNavigation.tab: cancelBtn
        KeyNavigation.up: cancelBtn
    }
}
