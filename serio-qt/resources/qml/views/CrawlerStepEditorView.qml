import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import '../widgets'

ViewLayout {
    onCreated: {
        cancelBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
        saveBtn.clicked.connect(() => actionRouter.trigger(ActionType.SAVE_CRAWLER_STEP, []))
        deleteBtn.clicked.connect(() => actionRouter.trigger(ActionType.REMOVE_CRAWLER_STEP, []))
        typeList.itemSelected.connect((name) => actionRouter.trigger(ActionType.SELECT_CRAWLER_STEP_TYPE, [name]))
    }
    onDisplayed: actionRouter.trigger(ActionType.LOAD_CRAWLER_STEP, [])
    AccentTitle {
        text: "Crawler step"
    }
    Row {
        Layout.fillWidth: true
        spacing: globalPadding
        SerioButton {
            id: cancelBtn
            text: "cancel"
            KeyNavigation.tab: saveBtn
            KeyNavigation.right: saveBtn
        }
        SerioButton {
            id: saveBtn
            text: "save"
            KeyNavigation.tab: deleteBtn
            KeyNavigation.right: deleteBtn
            KeyNavigation.down: typeList
        }
        SerioButton {
            id: deleteBtn
            text: "delete"
            enabled: crawlerStepEditorViewModel.isExistingStep
            KeyNavigation.tab: typeList
            KeyNavigation.down: typeList
        }
    }
    ItemWithTitle {
        title: "Type:"
        Layout.fillWidth: true
        DynamicRadioButtonList {
            id: typeList
            Layout.fillWidth: true
            height: globalPadding * 3
            focus: true
            model: crawlerStepEditorViewModel.crawlerStepTypes
            KeyNavigation.tab: propertiesList
            KeyNavigation.down: propertiesList
            KeyNavigation.up: cancelBtn
        }
    }
    ItemWithTitle {
        title: "Description:"
        Layout.fillWidth: true
        Paragraph {
            Layout.fillWidth: true
            Layout.fillHeight: !propertiesContainer.visible
            text: crawlerStepEditorViewModel.description
        }
    }
    ItemWithTitle {
        id: propertiesContainer
        title: "Properties:"
        visible: crawlerStepEditorViewModel.properties.length > 0
        Layout.fillWidth: true
        TextFieldList {
            id: propertiesList
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: crawlerStepEditorViewModel.properties
            KeyNavigation.tab: cancelBtn
        }
    }
}
