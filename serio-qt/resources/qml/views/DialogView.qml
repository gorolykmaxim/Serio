import QtQuick 2.12
import QtQuick.Layouts 1.12
import "../widgets"

CenteredViewLayout {
    layoutSpacing: globalPadding
    AccentTitle {
        Layout.fillWidth: true
        horizontalAlignment: Text.AlignHCenter
        text: dialogViewModel.title
    }
    Paragraph {
        Layout.fillWidth: true
        text: dialogViewModel.message
    }
    ColumnLayout {
        Layout.fillWidth: true
        spacing: 0
        SerioButton {
            id: topBtn
            Layout.fillWidth: true
            text: dialogViewModel.topButtonText
            focus: dialogViewModel.bottomButtonHidden
            onClicked: actionRouter.trigger(dialogViewModel.topButtonAction, [])
            KeyNavigation.tab: bottomBtn
            KeyNavigation.down: bottomBtn
        }
        SerioButton {
            id: bottomBtn
            Layout.fillWidth: true
            text: dialogViewModel.bottomButtonText
            focus: !dialogViewModel.bottomButtonHidden
            onClicked: actionRouter.trigger(dialogViewModel.bottomButtonAction, [])
            visible: !dialogViewModel.bottomButtonHidden
            KeyNavigation.tab: topBtn
        }
    }
}
