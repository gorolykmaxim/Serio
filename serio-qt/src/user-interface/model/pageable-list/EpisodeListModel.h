#ifndef SERIO_EPISODELISTMODEL_H
#define SERIO_EPISODELISTMODEL_H

#include <tv-show-storage/Episode.h>
#include "AbstractListModel.h"
#include "AbstractListModelServant.h"

namespace serio::qt {

class EpisodeListModel : public AbstractListModel {
public:
    enum Role {
        NAME, VIDEO_URL, LAST_WATCH_DATE
    };
    EpisodeListModel(unsigned int pageSize, unsigned int pageCountLimit);
    void loadPage(const core::ListPage<core::Episode>& page);
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
private:
    AbstractListModelServant<core::Episode> servant;
    const QHash<int, QByteArray> roleToName = {
        {Role::NAME, "name"},
        {Role::VIDEO_URL, "videoUrl"},
        {Role::LAST_WATCH_DATE, "lastWatchDate"}
    };
    QVariant getAttributeOf(const core::Episode& episode, int role) const;
};

}

#endif //SERIO_EPISODELISTMODEL_H
