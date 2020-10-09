#ifndef SERIO_ALLTVSHOWSVIEWMODEL_H
#define SERIO_ALLTVSHOWSVIEWMODEL_H

#include <map>
#include <user-interface/model/pageable-list/TvShowListModel.h>
#include <tv-show-storage/TvShowStorage.h>
#include <user-interface/action/ActionRouter.h>
#include <QQmlApplicationEngine>
#include <tv-show-viewer/TvShowViewer.h>
#include <user-interface/model/ButtonModel.h>
#include <user-interface/model/ListModel.h>
#include "ViewModel.h"

namespace serio::qt {

class AllTvShowsViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(QList<ButtonModel*> actions READ getActions NOTIFY actionsChanged)
    Q_PROPERTY(TvShowListModel* tvShowsList READ getTvShowList NOTIFY selectedListChanged)
public:
    inline static const QString allTvShowsAction = "All TV Shows";
    inline static const QString watchedAction = "Watched";
    inline static const QString addTvShowAction = "Add TV Show";
    AllTvShowsViewModel(unsigned int listModelPageSize, unsigned int listModelPageCountLimit, core::TvShowStorage& storage,
                        core::TvShowViewer& viewer, StackOfViews& stack);
    void initialize(ActionRouter& router, QQmlApplicationEngine& engine);
    void loadAllShows(const QVariantList& args);
    void loadWatchedShows(const QVariantList& args);
    void openTvShowView(const QVariantList& args);
    void loadFirstPage();
    QList<ButtonModel*> getActions();
    void selectAction(const QString& actionName);
    TvShowListModel* getTvShowList();
    void openAllTvShowsView();
signals:
    void actionsChanged() const;
    void selectedListChanged() const;
private:
    std::map<QString, std::unique_ptr<TvShowListModel>> listTypeToListModel;
    TvShowListModel* selectedTvShowList;
    ListModel<ButtonModel*> actions;
    core::TvShowStorage& storage;
    core::TvShowViewer& viewer;
    StackOfViews& stack;
    void addOrRemoveWatchedActionIfWatchedListSizeChanged(const core::ListPage<core::TvShow> &page);
    void addWatchedAction();
    void removeWatchedAction();
    void changeActionsHighlightment(const QString& selectedAction);
    TvShowListModel* getTvShowList(const QString& listType);
};

}


#endif //SERIO_ALLTVSHOWSVIEWMODEL_H
