#include "AllTvShowsViewModel.h"
#include <QQmlContext>

serio::qt::AllTvShowsViewModel::AllTvShowsViewModel(unsigned int listModelPageSize, unsigned int listModelPageCountLimit,
                                                    serio::core::TvShowStorage& storage, core::TvShowViewer& viewer,
                                                    serio::qt::StackOfViews& stack)
    : pageSize(listModelPageSize),
      allShowsListModel(listModelPageSize, listModelPageCountLimit),
      watchedShowsListModel(listModelPageSize, listModelPageCountLimit),
      storage(storage),
      viewer(viewer),
      stack(stack) {}

void serio::qt::AllTvShowsViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine &engine) {
    qmlRegisterUncreatableType<TvShowListModel>("Serio", 1, 0, "TvShowListModel", nullptr);
    engine.rootContext()->setContextProperty("allTvShowsViewModel", this);
    router.registerAction(ActionType::LOAD_ALL_TV_SHOWS_LIST_PAGE, [this] (const QVariantList& args) { loadAllShows(args); });
    router.registerAction(ActionType::LOAD_WATCHED_TV_SHOWS_LIST_PAGE, [this] (const QVariantList& args) { loadWatchedShows(args); });
    router.registerAction(ActionType::OPEN_TV_SHOW_VIEW, [this] (const QVariantList& args) { openTvShowView(args); });
}

unsigned int serio::qt::AllTvShowsViewModel::getPageSize() const {
    return pageSize;
}

serio::qt::TvShowListModel* serio::qt::AllTvShowsViewModel::getAllShows() {
    return &allShowsListModel;
}

serio::qt::TvShowListModel *serio::qt::AllTvShowsViewModel::getWatchedShows() {
    return &watchedShowsListModel;
}

void serio::qt::AllTvShowsViewModel::loadAllShows(const QVariantList& args) {
    core::ListPage<core::TvShow> page = storage.getAllTvShows(args[0].toUInt(), args[1].toUInt());
    modifyModel([page, this] { allShowsListModel.loadPage(page); });
}

void serio::qt::AllTvShowsViewModel::loadWatchedShows(const QVariantList& args) {
    core::ListPage<core::TvShow> page = storage.getWatchedTvShows(args[0].toUInt(), args[1].toUInt());
    modifyModel([page, this] { watchedShowsListModel.loadPage(page); });
}

void serio::qt::AllTvShowsViewModel::openTvShowView(const QVariantList &args) {
    viewer.openTvShowWithName(args[0].toString().toStdString());
    stack.pushView("TvShowView.qml");
}
