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
    public:
        AllTvShowsViewModel(unsigned int listModelPageSize,
                            unsigned int listModelPageCountLimit,
                            core::TvShowsFacade &tvShows, TaskExecutor& executor);
        TvShowListModel* getAllShows();
    signals:
        void allShowsListChanged(const TvShowListModel&);
    public slots:
        void reload();
    private:
        const unsigned int listModelPageSize;
        TvShowListModel allShowsListModel;
        TaskExecutor& executor;
        core::TvShowsFacade& tvShows;
        QFutureWatcher<core::ListPage<core::TvShow>> allShowsWatcher;
        void loadNextPage(unsigned int offset, unsigned int limit);
        void displayNextPage();
    };
}


#endif //SERIO_ALLTVSHOWSVIEWMODEL_H
