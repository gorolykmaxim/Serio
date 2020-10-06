import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import '../../lib/circular-progressbar'

CircularProgressBar {
    id: root
    value: 0
    lineWidth: 10
    animationDuration: 400
    primaryColor: Material.accent
    secondaryColor: "transparent"
    Timer {
        interval: 800
        running: root.visible
        repeat: true
        triggeredOnStart: true
        onTriggered: parent.value < 0.5 ? parent.value = 0.9 : parent.value = 0.001
    }
    RotationAnimator {
        target: root
        from: 0
        to: 360
        duration: 800
        running: root.visible
        loops: Animation.Infinite
    }
}
