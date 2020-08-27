#ifndef SERIO_ALLTVSHOWSVIEWMODEL_H
#define SERIO_ALLTVSHOWSVIEWMODEL_H

#include <QObject>
#include <model/TvShowListModel.h>
#include <storage/TvShowStorage.h>
#include <QFutureWatcher>
#include <concurrency/TaskExecutor.h>

namespace serio::qt {
    class AllTvShowsViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(TvShowListModel* allShowsList READ getAllShows NOTIFY allShowsListChanged)
    Q_PROPERTY(TvShowListModel* watchedShowsList READ getWatchedShows NOTIFY watchedShowsListChanged)
    public:
        AllTvShowsViewModel(unsigned int listModelPageSize,
                            unsigned int listModelPageCountLimit,
                            core::TvShowsFacade &tvShows, TaskExecutor& executor);
        TvShowListModel* getAllShows();
        TvShowListModel* getWatchedShows();
    signals:
        void allShowsListChanged(const TvShowListModel&);
        void watchedShowsListChanged(const TvShowListModel&);
    public slots:
        void reload();
    private:
        const unsigned int listModelPageSize;
        TvShowListModel allShowsListModel, watchedShowsListModel;
        TaskExecutor& executor;
        core::TvShowsFacade& tvShows;
        QFutureWatcher<core::ListPage<core::TvShow>> allShowsWatcher, watchedShowsWatcher;
        void loadNextPageOfAllShows(unsigned int offset, unsigned int limit);
        void loadNextPageOfWatchedShows(unsigned int offset, unsigned int limit);
        void displayNextPageOfAllShows();
        void displayNextPageOfWatchedShows();
    };
}


#endif //SERIO_ALLTVSHOWSVIEWMODEL_H
