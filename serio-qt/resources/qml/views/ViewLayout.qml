import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import '../widgets'

View {
    property string titleText: ""
    property string subtitleText: ""
    default property alias contents: placeholder.children
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: globalPadding
        spacing: globalPadding
        AccentTitleAndSubtitle{
            title: titleText
            subtitle: subtitleText
        }
        ColumnLayout {
            id: placeholder
            spacing: globalPadding
        }
    }
}
