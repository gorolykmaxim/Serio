import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import '../animations'
import '../../lib/QtMaterialDesignIcons/qml'

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
            MaterialDesignIcon {
                Layout.alignment: Qt.AlignVCenter
                name: !tileIcon || tileIcon == "placeholder" ? "bug" : tileIcon
                color: !tileIcon || tileIcon == "placeholder" ? "transparent" : Material.accent
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
