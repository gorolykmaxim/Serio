import QtQuick 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

ViewLayout {
    onCreated: backBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
    AccentTitle {
        text: "Crawl Log Entry Details"
    }
    RaisedButton {
        id: backBtn
        text: "back"
        focus: true
    }
    ScrollableColumn {
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: globalPadding
        ItemWithTitle {
            title: "Step"
            width: parent.width
            Paragraph {
                Layout.fillWidth: true
                text: crawlLogViewModel.selectedEntryText
            }
        }
        ItemWithTitle {
            title: "Input"
            width: parent.width
            Paragraph {
                Layout.fillWidth: true
                text: crawlLogViewModel.selectedEntryInputData
            }
        }
        ItemWithTitle {
            title: "Output"
            width: parent.width
            Paragraph {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: crawlLogViewModel.selectedEntryOutputData
            }
        }
    }
}
