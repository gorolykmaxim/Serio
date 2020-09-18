import QtQuick 2.12
import Serio 1.0

DialogView {
    dialogTitle: "Whoops..."
    dialogMessage: errorViewModel.errorText
    hideLeftButton: true
    rightButtonText: "understand"
    onRightButtonClicked: actionRouter.trigger(ActionType.BACK, [])
}
