#include "AllTvShowsViewModel.h"

serio::qt::AllTvShowsViewModel::AllTvShowsViewModel(unsigned int listModelPageSize,
                                                    unsigned int listModelPageCountLimit,
                                                    core::TvShowStorage& tvShows,
                                                    QTaskExecutor& executor)
                                                        : listModelPageSize(listModelPageSize),
                                                          allShowsListModel(listModelPageSize, listModelPageCountLimit),
                                                          watchedShowsListModel(listModelPageSize, listModelPageCountLimit),
                                                          tvShows(tvShows),
                                                          executor(executor){
    connect(&allShowsListModel, &TvShowListModel::requestPageLoad, this, &AllTvShowsViewModel::loadNextPageOfAllShows);
    connect(&allShowsWatcher, &QFutureWatcher<core::ListPage<core::TvShow>>::finished, this, &AllTvShowsViewModel::displayNextPageOfAllShows);
    connect(&watchedShowsListModel, &TvShowListModel::requestPageLoad, this, &AllTvShowsViewModel::loadNextPageOfWatchedShows);
    connect(&watchedShowsWatcher, &QFutureWatcher<core::ListPage<core::TvShow>>::finished, this, &AllTvShowsViewModel::displayNextPageOfWatchedShows);
}

serio::qt::TvShowListModel* serio::qt::AllTvShowsViewModel::getAllShows() {
    return &allShowsListModel;
}

serio::qt::TvShowListModel *serio::qt::AllTvShowsViewModel::getWatchedShows() {
    return &watchedShowsListModel;
}

void serio::qt::AllTvShowsViewModel::loadNextPageOfAllShows(unsigned int offset, unsigned int limit) {
    executor.runInBackground(allShowsWatcher, &tvShows, &core::TvShowStorage::getAllTvShows, offset, limit);
}

void serio::qt::AllTvShowsViewModel::loadNextPageOfWatchedShows(unsigned int offset, unsigned int limit) {
    executor.runInBackground(watchedShowsWatcher, &tvShows, &core::TvShowStorage::getWatchedTvShows, offset, limit);
}

void serio::qt::AllTvShowsViewModel::displayNextPageOfAllShows() {
    allShowsListModel.loadPage(allShowsWatcher.result());
}

void serio::qt::AllTvShowsViewModel::displayNextPageOfWatchedShows() {
    watchedShowsListModel.loadPage(watchedShowsWatcher.result());
}

void serio::qt::AllTvShowsViewModel::reload() {
    loadNextPageOfAllShows(0, listModelPageSize);
    emit allShowsListChanged(allShowsListModel);
    loadNextPageOfWatchedShows(0, listModelPageSize);
    emit watchedShowsListChanged(watchedShowsListModel);
}
