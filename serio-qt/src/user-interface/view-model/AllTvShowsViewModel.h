#ifndef SERIO_ALLTVSHOWSVIEWMODEL_H
#define SERIO_ALLTVSHOWSVIEWMODEL_H

#include <user-interface/model/TvShowListModel.h>
#include <tv-show-storage/TvShowStorage.h>
#include <user-interface/action/ActionRouter.h>
#include <QQmlApplicationEngine>
#include "ViewModel.h"

namespace serio::qt {

class AllTvShowsViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(unsigned int pageSize READ getPageSize CONSTANT)
    Q_PROPERTY(TvShowListModel* allShowsList READ getAllShows CONSTANT)
    Q_PROPERTY(TvShowListModel* watchedShowsList READ getWatchedShows CONSTANT)
public:
    AllTvShowsViewModel(unsigned int listModelPageSize, unsigned int listModelPageCountLimit, core::TvShowStorage& storage);
    void initialize(ActionRouter& router, QQmlApplicationEngine& engine);
    unsigned int getPageSize() const;
    TvShowListModel* getAllShows();
    TvShowListModel* getWatchedShows();
    void loadAllShows(const QVariantList& args);
    void loadWatchedShows(const QVariantList& args);
private:
    unsigned int pageSize;
    TvShowListModel allShowsListModel, watchedShowsListModel;
    core::TvShowStorage& storage;
};

}


#endif //SERIO_ALLTVSHOWSVIEWMODEL_H
