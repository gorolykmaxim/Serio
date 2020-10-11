import QtQuick 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import '../widgets'

ViewLayout {
    onCreated: {
        tvShowsList.tvShowSelected.connect((tvShowName) => actionRouter.trigger(ActionType.OPEN_TV_SHOW_VIEW, [tvShowName]))
        displayed()
    }
    onDisplayed: actionRouter.trigger(ActionType.LOAD_FIRST_PAGE_OF_TV_SHOWS, [])
    NavButtonList {
        id: actionsList
        Layout.fillWidth: true
        spacing: globalPadding
        focus: true
        model: allTvShowsViewModel.actions
        KeyNavigation.down: tvShowsList
        KeyNavigation.tab: tvShowsList
    }
    TvShowCardList {
        id: tvShowsList
        singleCardOnScreen: height > width
        Layout.fillHeight: true
        Layout.fillWidth: true
        cardSpacing: globalPadding
        visible: count > 0
        listModel: allTvShowsViewModel.tvShowsList
        KeyNavigation.tab: actionsList
    }
    NoContentPlaceholder {
        contentType: "tv shows"
        Layout.fillHeight: true
        Layout.fillWidth: true
        visible: tvShowsList.count === 0
    }
}
