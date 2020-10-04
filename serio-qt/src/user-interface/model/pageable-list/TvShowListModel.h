#ifndef SERIO_TVSHOWLISTMODEL_H
#define SERIO_TVSHOWLISTMODEL_H

#include <tv-show-storage/TvShow.h>
#include "AbstractListModel.h"
#include "AbstractListModelServant.h"

namespace serio::qt {

class TvShowListModel : public AbstractListModel {
    Q_OBJECT
public:
    enum Role {
        NAME, THUMBNAIL_URL, LAST_WATCH_DATE
    };
    TvShowListModel(unsigned int pageSize, unsigned int pageCountLimit);
    void loadPage(const core::ListPage<core::TvShow>& page);
    void requestFirstPageLoad() const;
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
private:
    AbstractListModelServant<core::TvShow> servant;
    QVariant getAttributeOf(const core::TvShow& tvShow, int role) const;
    QString getLastWatchDate(const std::optional<core::LastWatchDate>& lastWatchDate) const;
};

}

#endif //SERIO_TVSHOWLISTMODEL_H
