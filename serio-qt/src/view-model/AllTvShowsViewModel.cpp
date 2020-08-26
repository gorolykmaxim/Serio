#include "AllTvShowsViewModel.h"

serio::qt::AllTvShowsViewModel::AllTvShowsViewModel(unsigned int listModelPageSize,
                                                    unsigned int listModelPageCountLimit,
                                                    core::TvShowsFacade& tvShows,
                                                    TaskExecutor& executor)
                                                        : listModelPageSize(listModelPageSize),
                                                          allShowsListModel(listModelPageSize, listModelPageCountLimit),
                                                          tvShows(tvShows),
                                                          executor(executor){
    connect(&allShowsListModel, &TvShowListModel::requestPageLoad, this, &AllTvShowsViewModel::loadNextPage);
    connect(&allShowsWatcher, &QFutureWatcher<core::ListPage<core::TvShow>>::finished, this, &AllTvShowsViewModel::displayNextPage);
}

serio::qt::TvShowListModel* serio::qt::AllTvShowsViewModel::getAllShows() {
    return &allShowsListModel;
}

void serio::qt::AllTvShowsViewModel::loadNextPage(unsigned int offset, unsigned int limit) {
    executor.runInBackground(allShowsWatcher, &tvShows, &core::TvShowsFacade::getAllTvShows, offset, limit);
}

void serio::qt::AllTvShowsViewModel::displayNextPage() {
    allShowsListModel.loadPage(allShowsWatcher.result());
}

void serio::qt::AllTvShowsViewModel::reload() {
    loadNextPage(0, listModelPageSize);
}
