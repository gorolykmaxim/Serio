import QtQuick 2.12

Subtitle {
    property string contentType
    text: "No " + contentType + " found =("
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
}
