import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "../../lib/QtMaterialDesignIcons/qml/MaterialDesignIconGlyphs.js" as MaterialGlyphs

RoundButton {
    property string iconName
    font.family: materialFont.name
    font.pixelSize: 24
    text: MaterialGlyphs.glyphs[iconName]
    Material.foreground: Material.accent
    Keys.onReturnPressed: clicked()
    Keys.onEnterPressed: clicked()
    FontLoader {
        id: materialFont
        source: "qrc:/materialdesignicons-webfont.ttf"
    }
}
