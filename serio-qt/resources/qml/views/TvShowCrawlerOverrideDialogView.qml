import QtQuick 2.12
import Serio 1.0

DialogView {
    dialogTitle: "TV-show '" + tvShowCrawlerEditorViewModel.tvShowName + "' already exists"
    dialogMessage: "Are you sure you want to override the existing crawler and the show with the new ones?"
    onLeftButtonClicked: actionRouter.trigger(ActionType.BACK, [])
    onRightButtonClicked: actionRouter.trigger(ActionType.SAVE_TV_SHOW_CRAWLER_WITH_OVERRIDE, [])
}
