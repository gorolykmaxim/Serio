import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import '../animations'

ItemDelegate {
    property string title
    property string subtitle
    property real tilePadding: 8
    width: parent.width
    height: crawlerStepItem.height
    Keys.onReturnPressed: clicked()
    Keys.onEnterPressed: clicked()
    Column {
        id: crawlerStepItem
        padding: tilePadding
        Subtitle {
            text: title
        }
        GreySubtitle {
            text: subtitle
            visible: subtitle !== null
        }
    }
}
