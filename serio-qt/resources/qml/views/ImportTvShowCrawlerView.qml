import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

ViewLayout {
    onCreated: {
        cancelBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
        importBtn.clicked.connect(() => {
            actionRouter.trigger(ActionType.SET_RAW_TV_SHOW_CRAWLER, [textArea.text])
            actionRouter.trigger(ActionType.SAVE_TV_SHOW_CRAWLER, [])
        })
    }
    AccentTitle {
        text: "Import TV Show crawler"
    }
    Row {
        Layout.fillWidth: true
        spacing: globalPadding
        SerioButton {
            id: cancelBtn
            text: "cancel"
            KeyNavigation.right: importBtn
            KeyNavigation.down: textArea
            KeyNavigation.tab: importBtn
        }
        SerioButton {
            id: importBtn
            text: "import"
            KeyNavigation.down: textArea
            KeyNavigation.tab: textArea
        }
    }
    Flickable {
        Layout.fillWidth: true
        Layout.fillHeight: true
        TextArea.flickable: TextArea {
            id: textArea
            focus: true
            placeholderText: "Paste configuration of a tv show crawler here"
            KeyNavigation.priority: KeyNavigation.BeforeItem
            KeyNavigation.tab: cancelBtn
        }
        ScrollBar.vertical: ScrollBar {}
    }
}
