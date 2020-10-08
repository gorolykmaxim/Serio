import QtQuick 2.12
import "../../lib/qml-material-icons/src/MaterialDesign.js" as MD

Text {
    property string iconName
    width: 24
    height: width
    font.family: materialFont.name
    font.pixelSize: height
    text: MD.icons[iconName]
    FontLoader {
        id: materialFont
        source: "qrc:/lib/qml-material-icons/fonts/MaterialIcons-Regular.ttf"
    }
}
