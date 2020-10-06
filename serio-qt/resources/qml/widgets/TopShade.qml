import QtQuick 2.0

Rectangle {
    property bool display: false
    gradient: Gradient {
        GradientStop {position: 0; color: "black"}
        GradientStop {position: 0.4; color: "transparent"}
    }
    opacity: display ? 0.6 : 0
}
