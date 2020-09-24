import QtQuick 2.12

Column {
    property string title
    property string subtitle
    AccentTitle {
        text: title
    }
    Subtitle {
        text: subtitle
        visible: subtitle
    }
}
