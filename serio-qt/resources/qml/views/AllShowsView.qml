import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Serio 1.0
import "../widgets"

ViewLayout {
    onCreated: {
        allTvShowsViewModel.allShowsList.requestPageLoad.connect((offset, limit) => actionRouter.trigger(ActionType.LOAD_ALL_TV_SHOWS_LIST_PAGE, [offset, limit]))
        allTvShowsViewModel.watchedShowsList.requestPageLoad.connect((offset, limit) => actionRouter.trigger(ActionType.LOAD_WATCHED_TV_SHOWS_LIST_PAGE, [offset, limit]))
        addTvShowBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_ADD_TV_SHOW_VIEW, []))
        watchedShowsList.tvShowSelected.connect((tvShowName) => actionRouter.trigger(ActionType.OPEN_TV_SHOW_VIEW, [tvShowName]))
        allShowsLists.tvShowSelected.connect((tvShowName) => actionRouter.trigger(ActionType.OPEN_TV_SHOW_VIEW, [tvShowName]))
        displayed()
    }
    onDisplayed: actionRouter.trigger(ActionType.LOAD_FIRST_PAGE_OF_TV_SHOWS, [])
    AccentTitle {
        text: "Last Watched"
        visible: watchedShowsList.visible
    }
    TvShowCardList {
        id: watchedShowsList
        Layout.fillHeight: true
        Layout.fillWidth: true
        cardSpacing: globalPadding
        focus: visible
        visible: count > 0
        listModel: allTvShowsViewModel.watchedShowsList
        KeyNavigation.tab: addTvShowBtn
        KeyNavigation.down: addTvShowBtn
    }
    SerioButton {
        id: addTvShowBtn
        focus: !watchedShowsList.visible
        text: "add tv show"
        KeyNavigation.tab: allShowsLists
        KeyNavigation.down: allShowsLists
    }
    TvShowCardList {
        id: allShowsLists
        Layout.fillHeight: true
        Layout.fillWidth: true
        cardSpacing: globalPadding
        listModel: allTvShowsViewModel.allShowsList
        KeyNavigation.tab: watchedShowsList
    }
}
