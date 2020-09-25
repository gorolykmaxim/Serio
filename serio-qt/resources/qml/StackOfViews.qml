import QtQuick 2.12
import QtQuick.Controls 2.12

StackView {
    id: root
    anchors.fill: parent
    focus: true
    Connections {
        target: stackOfViews
        function onPush(view) {
            root.push(view)
        }
        function onPop(currentView) {
            if (currentView)
                root.pop()
            else
                root.pop(null)
        }
        function onReplace(newView) {
            root.replace(newView)
        }
    }
}
