import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "../../lib/QtMaterialDesignIcons/qml/MaterialDesignIconGlyphs.js" as MaterialGlyphs

RoundButton {
    property string iconName
    signal gotFocus()
    font.family: materialFont.name
    font.pixelSize: 28
    text: MaterialGlyphs.glyphs[iconName]
    highlighted: activeFocus
    Material.foreground: Material.accent
    Keys.onReturnPressed: clicked()
    Keys.onEnterPressed: clicked()
    onFocusChanged: activeFocus && gotFocus()
    FontLoader {
        id: materialFont
        source: "qrc:/materialdesignicons-webfont.ttf"
    }
}
