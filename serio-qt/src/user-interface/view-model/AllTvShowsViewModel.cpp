#include "AllTvShowsViewModel.h"
#include <QQmlContext>
#include <user-interface/ViewNames.h>

serio::qt::AllTvShowsViewModel::AllTvShowsViewModel(unsigned int listModelPageSize, unsigned int listModelPageCountLimit,
                                                    serio::core::TvShowStorage& storage, core::TvShowViewer& viewer,
                                                    serio::qt::StackOfViews& stack)
    : storage(storage),
      viewer(viewer),
      stack(stack) {
    listTypeToListModel.emplace(allTvShowsAction, std::make_unique<serio::qt::TvShowListModel>(listModelPageSize, listModelPageCountLimit));
    listTypeToListModel.emplace(watchedAction, std::make_unique<serio::qt::TvShowListModel>(listModelPageSize, listModelPageCountLimit));
    actions << new serio::qt::ButtonModel(watchedAction, serio::qt::ActionType::DISPLAY_WATCHED_TV_SHOWS);
    actions << new serio::qt::ButtonModel(allTvShowsAction, serio::qt::ActionType::DISPLAY_ALL_TV_SHOWS);
    actions << new serio::qt::ButtonModel(addTvShowAction, serio::qt::ActionType::OPEN_ADD_TV_SHOW_VIEW);
    selectAction(watchedAction);
}

void serio::qt::AllTvShowsViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine &engine) {
    qmlRegisterUncreatableType<TvShowListModel>("Serio", 1, 0, "TvShowListModel", nullptr);
    engine.rootContext()->setContextProperty("allTvShowsViewModel", this);
    router.registerAction(ActionType::DISPLAY_ALL_TV_SHOWS, [this] (const auto& args) { selectAction(allTvShowsAction); });
    router.registerAction(ActionType::DISPLAY_WATCHED_TV_SHOWS, [this] (const auto& args) { selectAction(watchedAction); });
    router.registerAction(ActionType::LOAD_ALL_TV_SHOWS_LIST_PAGE, [this] (const auto& args) { loadAllShows(args); });
    router.registerAction(ActionType::LOAD_WATCHED_TV_SHOWS_LIST_PAGE, [this] (const auto& args) { loadWatchedShows(args); });
    router.registerAction(ActionType::OPEN_TV_SHOW_VIEW, [this] (const auto& args) { openTvShowView(args); });
    router.registerAction(ActionType::LOAD_FIRST_PAGE_OF_TV_SHOWS, [this] (const auto& args) { loadFirstPage(); });
    connect(getTvShowList(allTvShowsAction), &serio::qt::TvShowListModel::requestPageLoad,
            this, [&router] (auto offset, auto limit) { router.trigger(serio::qt::ActionType::LOAD_ALL_TV_SHOWS_LIST_PAGE, QVariantList({offset, limit})); });
    connect(getTvShowList(watchedAction), &serio::qt::TvShowListModel::requestPageLoad,
            this, [&router] (auto offset, auto limit) { router.trigger(serio::qt::ActionType::LOAD_WATCHED_TV_SHOWS_LIST_PAGE, QVariantList({offset, limit})); });
    loadFirstPage();
}

void serio::qt::AllTvShowsViewModel::loadAllShows(const QVariantList& args) {
    auto page = storage.getAllTvShows(args[0].toUInt(), args[1].toUInt());
    modifyModel([page, this] { getTvShowList(allTvShowsAction)->loadPage(page); });
}

void serio::qt::AllTvShowsViewModel::loadWatchedShows(const QVariantList& args) {
    auto page = storage.getWatchedTvShows(args[0].toUInt(), args[1].toUInt());
    modifyModel([page, this] {
        addOrRemoveWatchedActionIfWatchedListSizeChanged(page);
        getTvShowList(watchedAction)->loadPage(page);
    });
}

void serio::qt::AllTvShowsViewModel::openTvShowView(const QVariantList &args) {
    viewer.openTvShowWithName(args[0].toString().toStdString());
    stack.pushView(tvShowView);
}

void serio::qt::AllTvShowsViewModel::loadFirstPage() {
    modifyModel([this] {
        getTvShowList(allTvShowsAction)->requestFirstPageLoad();
        getTvShowList(watchedAction)->requestFirstPageLoad();
    });
}

QList<serio::qt::ButtonModel*> serio::qt::AllTvShowsViewModel::getActions() {
    return actions;
}

void serio::qt::AllTvShowsViewModel::addOrRemoveWatchedActionIfWatchedListSizeChanged(const core::ListPage<core::TvShow> &page) {
    auto isFirstActionWatched = actions[0]->getText() == watchedAction;
    if (page.getTotalSize() > 0 && !isFirstActionWatched) {
        addWatchedAction();
    } else if (page.getTotalSize() == 0 && isFirstActionWatched) {
        removeWatchedAction();
    }
}

void serio::qt::AllTvShowsViewModel::addWatchedAction() {
    actions.insert(0, new serio::qt::ButtonModel(watchedAction, serio::qt::ActionType::DISPLAY_WATCHED_TV_SHOWS));
    selectAction(watchedAction);
    emit actionsChanged();
}

void serio::qt::AllTvShowsViewModel::removeWatchedAction() {
    actions.removeAndDelete(0);
    selectAction(allTvShowsAction);
    emit actionsChanged();
}

void serio::qt::AllTvShowsViewModel::selectAction(const QString& actionName) {
    changeActionsHighlightment(actionName);
    selectedTvShowList = getTvShowList(actionName);
    emit selectedListChanged();
}

void serio::qt::AllTvShowsViewModel::changeActionsHighlightment(const QString &selectedAction) {
    for (auto action: actions) {
        action->setHighlighted(action->getText() == selectedAction);
    }
}

serio::qt::TvShowListModel *serio::qt::AllTvShowsViewModel::getTvShowList() {
    return selectedTvShowList;
}

serio::qt::TvShowListModel *serio::qt::AllTvShowsViewModel::getTvShowList(const QString &listType) {
    return listTypeToListModel[listType].get();
}
