import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

CenteredViewLayout {
    onCreated: {
        addTvShowBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_TV_SHOW_CRAWLER_EDITOR_VIEW, []))
        importTvShowBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_IMPORT_TV_SHOW_VIEW, []))
        cancelBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
    }
    titleText: "Add TV Show Options"
    RaisedButton {
        id: addTvShowBtn
        Layout.fillWidth: true
        focus: true
        text: "add tv show"
        KeyNavigation.tab: importTvShowBtn
        KeyNavigation.down: importTvShowBtn
    }
    RaisedButton {
        id: importTvShowBtn
        Layout.fillWidth: true
        text: "import tv show"
        KeyNavigation.tab: cancelBtn
        KeyNavigation.down: cancelBtn
    }
    RaisedButton {
        id: cancelBtn
        primary: false
        Layout.fillWidth: true
        text: "cancel"
        KeyNavigation.tab: addTvShowBtn
    }
}
