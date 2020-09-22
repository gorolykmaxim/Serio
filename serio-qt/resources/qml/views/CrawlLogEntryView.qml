import QtQuick 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

ViewLayout {
    onCreated: backBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
    AccentTitle {
        text: "Crawl Log Entry Details"
    }
    SerioButton {
        id: backBtn
        text: "back"
        focus: true
    }
    ItemWithTitle {
        title: "Step"
        Layout.fillWidth: true
        Paragraph {
            Layout.fillWidth: true
            text: crawlLogViewModel.selectedEntryText
        }
    }
    ItemWithTitle {
        title: "Input"
        Layout.fillWidth: true
        Paragraph {
            Layout.fillWidth: true
            text: crawlLogViewModel.selectedEntryInputData
        }
    }
    ItemWithTitle {
        title: "Output"
        Layout.fillWidth: true
        Layout.fillHeight: true
        Paragraph {
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: crawlLogViewModel.selectedEntryOutputData
        }
    }
}
