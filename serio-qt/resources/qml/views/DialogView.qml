import QtQuick 2.12
import QtQuick.Layouts 1.12
import "../widgets"

CenteredViewLayout {
    layoutSpacing: globalPadding
    AccentTitle {
        Layout.fillWidth: true
        text: dialogViewModel.title
    }
    Paragraph {
        Layout.maximumWidth: 400
        text: dialogViewModel.message
    }
    RightToLeftButtonRow {
        Layout.fillWidth: true
        spacing: globalPadding
        SerioButton {
            id: rightBtn
            text: dialogViewModel.rightButtonText
            focus: dialogViewModel.leftButtonHidden
            onClicked: actionRouter.trigger(dialogViewModel.rightButtonAction, [])
            KeyNavigation.tab: leftBtn
        }
        SerioButton {
            id: leftBtn
            text: dialogViewModel.leftButtonText
            focus: !dialogViewModel.leftButtonHidden
            onClicked: actionRouter.trigger(dialogViewModel.leftButtonAction, [])
            visible: !dialogViewModel.leftButtonHidden
            KeyNavigation.tab: rightBtn
            KeyNavigation.right: rightBtn
        }
    }
}
