#include "AllTvShowsViewModel.h"
#include <QQmlContext>
#include <user-interface/ViewNames.h>

namespace serio::qt {

AllTvShowsViewModel::AllTvShowsViewModel(unsigned int listModelPageSize, unsigned int listModelPageCountLimit,
                                         core::TvShowStorage& storage, core::TvShowViewer& viewer, StackOfViews& stack)
    : storage(storage),
      viewer(viewer),
      stack(stack) {
    listTypeToListModel.emplace(allTvShowsAction, std::make_unique<TvShowListModel>(listModelPageSize, listModelPageCountLimit));
    listTypeToListModel.emplace(watchedAction, std::make_unique<TvShowListModel>(listModelPageSize, listModelPageCountLimit));
    actions << new ButtonModel(watchedAction, ActionType::DISPLAY_WATCHED_TV_SHOWS);
    actions << new ButtonModel(allTvShowsAction, ActionType::DISPLAY_ALL_TV_SHOWS);
    actions << new ButtonModel(addTvShowAction, ActionType::OPEN_ADD_TV_SHOW_VIEW);
    selectAction(watchedAction);
}

void AllTvShowsViewModel::initialize(ActionRouter &router, QQmlApplicationEngine &engine) {
    qmlRegisterUncreatableType<TvShowListModel>("Serio", 1, 0, "TvShowListModel", nullptr);
    engine.rootContext()->setContextProperty("allTvShowsViewModel", this);
    router.registerAction(ActionType::DISPLAY_ALL_TV_SHOWS, [this] (const auto& args) { selectAction(allTvShowsAction); });
    router.registerAction(ActionType::DISPLAY_WATCHED_TV_SHOWS, [this] (const auto& args) { selectAction(watchedAction); });
    router.registerAction(ActionType::LOAD_ALL_TV_SHOWS_LIST_PAGE, [this] (const auto& args) { loadAllShows(args); });
    router.registerAction(ActionType::LOAD_WATCHED_TV_SHOWS_LIST_PAGE, [this] (const auto& args) { loadWatchedShows(args); });
    router.registerAction(ActionType::OPEN_TV_SHOW_VIEW, [this] (const auto& args) { openTvShowView(args); });
    router.registerAction(ActionType::LOAD_FIRST_PAGE_OF_TV_SHOWS, [this] (const auto& args) { loadFirstPage(); });
    router.registerAction(ActionType::GO_TO_ALL_TV_SHOWS, [this] (const auto& args) { openAllTvShowsView(); });
    connect(getTvShowList(allTvShowsAction), &TvShowListModel::requestPageLoad,
            this, [&router] (auto offset, auto limit) { router.trigger(ActionType::LOAD_ALL_TV_SHOWS_LIST_PAGE, QVariantList({offset, limit})); });
    connect(getTvShowList(watchedAction), &TvShowListModel::requestPageLoad,
            this, [&router] (auto offset, auto limit) { router.trigger(ActionType::LOAD_WATCHED_TV_SHOWS_LIST_PAGE, QVariantList({offset, limit})); });
    loadFirstPage();
}

void AllTvShowsViewModel::loadAllShows(const QVariantList& args) {
    auto page = storage.getAllTvShows(args[0].toUInt(), args[1].toUInt());
    modifyModel([page, this] { getTvShowList(allTvShowsAction)->loadPage(page); });
}

void AllTvShowsViewModel::loadWatchedShows(const QVariantList& args) {
    auto page = storage.getWatchedTvShows(args[0].toUInt(), args[1].toUInt());
    modifyModel([page, this] {
        addOrRemoveWatchedActionIfWatchedListSizeChanged(page);
        getTvShowList(watchedAction)->loadPage(page);
    });
}

void AllTvShowsViewModel::openTvShowView(const QVariantList &args) {
    viewer.openTvShowWithName(args[0].toString().toStdString());
    stack.pushView(tvShowView);
}

void AllTvShowsViewModel::loadFirstPage() {
    modifyModel([this] {
        getTvShowList(allTvShowsAction)->requestFirstPageLoad();
        getTvShowList(watchedAction)->requestFirstPageLoad();
    });
}

QList<ButtonModel*> AllTvShowsViewModel::getActions() {
    return actions;
}

void AllTvShowsViewModel::addOrRemoveWatchedActionIfWatchedListSizeChanged(const core::ListPage<core::TvShow> &page) {
    auto isFirstActionWatched = actions[0]->getText() == watchedAction;
    if (page.getTotalSize() > 0 && !isFirstActionWatched) {
        addWatchedAction();
    } else if (page.getTotalSize() == 0 && isFirstActionWatched) {
        removeWatchedAction();
    }
}

void AllTvShowsViewModel::addWatchedAction() {
    actions.insert(0, new ButtonModel(watchedAction, ActionType::DISPLAY_WATCHED_TV_SHOWS));
    selectAction(watchedAction);
    emit actionsChanged();
}

void AllTvShowsViewModel::removeWatchedAction() {
    actions.removeAndDelete(0);
    selectAction(allTvShowsAction);
    emit actionsChanged();
}

void AllTvShowsViewModel::selectAction(const QString& actionName) {
    changeActionsHighlightment(actionName);
    selectedTvShowList = getTvShowList(actionName);
    emit selectedListChanged();
}

void AllTvShowsViewModel::changeActionsHighlightment(const QString &selectedAction) {
    for (auto action: actions) {
        action->setHighlighted(action->getText() == selectedAction);
    }
}

TvShowListModel *AllTvShowsViewModel::getTvShowList() {
    return selectedTvShowList;
}

TvShowListModel *AllTvShowsViewModel::getTvShowList(const QString &listType) {
    return listTypeToListModel[listType].get();
}

void AllTvShowsViewModel::openAllTvShowsView() {
    stack.popAllViews();
    selectAction(allTvShowsAction);
}

}