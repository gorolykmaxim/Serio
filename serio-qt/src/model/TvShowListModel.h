#ifndef SERIO_TVSHOWLISTMODEL_H
#define SERIO_TVSHOWLISTMODEL_H

#include <set>
#include <tv-shows/TvShowsFacade.h>
#include <QAbstractListModel>
#include <tv-shows/BigListProxy.h>

namespace serio::qt {

class TvShowListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Role {
        NAME, THUMBNAIL_URL, LAST_WATCH_DATE
    };

    TvShowListModel(unsigned int pageSize, unsigned int pageCountLimit);
    void loadPage(const core::ListPage<core::TvShow>& page);
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
signals:
    void requestPageLoad(unsigned int offset, unsigned int size) const;
private:
    const unsigned int pageSize;
    mutable std::set<unsigned int> loadingOffsets;
    serio::core::BigListProxy<serio::core::TvShow> listProxy;
    const QHash<int, QByteArray> roleToName = {
            {Role::NAME, "name"},
            {Role::THUMBNAIL_URL, "thumbnailUrl"},
            {Role::LAST_WATCH_DATE, "lastWatchDate"}
    };
    void extendModelIfNecessary(const core::ListPage<core::TvShow>& page);
    void shrinkModelIfNecessary(const core::ListPage<core::TvShow>& page);
    void pageIsLoaded(const core::ListPage<core::TvShow>& page);
    void loadPageWithIndex(unsigned int index) const;
    QVariant getAttributeOf(const core::TvShow& tvShow, int role) const;
};

}

#endif //SERIO_TVSHOWLISTMODEL_H
