import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property string imageUrl
    property real borderRadius
    property size imageSourceSize
    Image {
        id: image
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        sourceSize: imageSourceSize
        source: imageUrl
        visible: false
    }
    NoImagePlaceholder {
        id: noImagePlaceHolder
        anchors.fill: parent
        visible: false
    }
    Rectangle {
        id: roundedCorners
        anchors.fill: parent
        radius: borderRadius
        visible: false
    }
    OpacityMask {
        anchors.fill: parent
        source: imageUrl ? image : noImagePlaceHolder
        maskSource: roundedCorners
    }
}
