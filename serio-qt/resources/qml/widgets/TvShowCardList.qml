import QtQuick 2.0
import QtQuick.Window 2.12
import Serio 1.0

HorizontalCardList {
    property TvShowListModel listModel
    id: root
    model: listModel
    delegate: TvShowCard {
        height: root.height
        tvShowName: model.name || null
        thumbnailUrl: model.thumbnailUrl || null
        lastWatchDate: model.lastWatchDate || null
        thumbnailSourceHeight: Screen.desktopAvailableHeight
    }
}
