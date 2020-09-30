import QtQuick 2.0
import Serio 1.0

HorizontalCardList {
    property TvShowListModel listModel
    property bool singleCardOnScreen: false
    signal tvShowSelected(string tvShowName)
    id: root
    model: listModel
    delegate: TvShowCard {
        width: singleCardOnScreen ? root.width : 300
        height: root.height
        tvShowName: model.name
        thumbnailUrl: model.thumbnailUrl
        lastWatchDate: model.lastWatchDate
        onSelected: tvShowSelected(model.name)
    }
}
