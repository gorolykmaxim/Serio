import QtQuick 2.12
import QtQuick.Controls 2.12

StackView {
    id: root
    anchors.fill: parent
    focus: true
    Connections {
        target: stackOfViews
        function onPush(views) {
            root.push.apply(root, views)
        }
        function onPop(currentView) {
            if (currentView)
                root.pop()
            else
                root.pop(null)
        }
    }
}
