import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import '../animations'

ItemDelegate {
    property string title
    property string subtitle
    property string tileIcon
    property real tilePadding: 8
    height: root.height
    Keys.onReturnPressed: clicked()
    Keys.onEnterPressed: clicked()
    Row {
        id: root
        padding: tilePadding
        RowLayout {
            spacing: tilePadding
            MaterialIcon {
                Layout.alignment: Qt.AlignVCenter
                color: !tileIcon || tileIcon == "placeholder" ? "transparent" : Material.accent
                iconName: !tileIcon || tileIcon == "placeholder" ? "bug_report" : tileIcon
                visible: tileIcon
            }
            Column {
                Subtitle {
                    text: title
                }
                GreySubtitle {
                    text: subtitle
                    visible: subtitle
                }
            }
        }
    }
}
