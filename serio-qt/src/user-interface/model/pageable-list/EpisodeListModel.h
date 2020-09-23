#ifndef SERIO_EPISODELISTMODEL_H
#define SERIO_EPISODELISTMODEL_H

#include <tv-show-storage/Episode.h>
#include "AbstractListModel.h"
#include "AbstractListModelServant.h"

namespace serio::qt {

class EpisodeListModel : public AbstractListModel {
public:
    enum Role {
        TITLE, SUBTITLE, ICON
    };
    EpisodeListModel(unsigned int pageSize, unsigned int pageCountLimit);
    void loadPage(const core::ListPage<core::Episode>& page);
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
private:
    AbstractListModelServant<core::Episode> servant;
    QVariant getAttributeOf(const core::Episode& episode, int role) const;
};

}

#endif //SERIO_EPISODELISTMODEL_H
