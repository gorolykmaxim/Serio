import QtQuick 2.12
import QtQuick.Layouts 1.12
import "../widgets"

CenteredViewLayout {
    property string dialogTitle
    property string dialogMessage
    property string rightButtonText: "confirm"
    property string leftButtonText: "cancel"
    property bool hideLeftButton: false
    signal rightButtonClicked()
    signal leftButtonClicked()
    layoutSpacing: globalPadding
    AccentTitle {
        Layout.fillWidth: true
        text: dialogTitle
    }
    Paragraph {
        Layout.maximumWidth: 400
        text: dialogMessage
    }
    RightToLeftButtonRow {
        Layout.fillWidth: true
        spacing: globalPadding
        SerioButton {
            id: rightBtn
            text: rightButtonText
            onClicked: rightButtonClicked()
            KeyNavigation.tab: leftBtn
        }
        SerioButton {
            id: leftBtn
            text: leftButtonText
            focus: true
            onClicked: leftButtonClicked()
            visible: !hideLeftButton
            KeyNavigation.tab: rightBtn
            KeyNavigation.right: rightBtn
        }
    }
}
