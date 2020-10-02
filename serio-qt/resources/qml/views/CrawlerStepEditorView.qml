import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import '../widgets'

ViewLayout {
    onCreated: typeList.itemSelected.connect((name) => actionRouter.trigger(ActionType.SELECT_CRAWLER_STEP_TYPE, [name]))
    onDisplayed: actionRouter.trigger(ActionType.LOAD_CRAWLER_STEP, [])
    AccentTitle {
        text: "Crawler Step"
    }
    ButtonList {
        id: actionsList
        Layout.fillWidth: true
        spacing: globalPadding
        focus: true
        model: crawlerStepEditorViewModel.actions
        KeyNavigation.down: typeList
        KeyNavigation.tab: typeList
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
            KeyNavigation.up: actionsList
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
            KeyNavigation.tab: actionsList
        }
    }
}
