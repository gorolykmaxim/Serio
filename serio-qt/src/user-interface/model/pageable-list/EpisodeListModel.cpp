#include "EpisodeListModel.h"
#include "InvalidListRole.h"

namespace serio::qt {

EpisodeListModel::EpisodeListModel(unsigned int pageSize, unsigned int pageCountLimit)
    : AbstractListModel({
        {Role::TITLE,"title"},
        {Role::SUBTITLE,"subtitle"},
        {Role::ICON, "icon"}
    }), servant(pageSize, pageCountLimit, *this) {}

void EpisodeListModel::loadPage(const core::ListPage<core::Episode> &page) {
    servant.loadPage(page);
}

void EpisodeListModel::requestFirstPageLoad() const {
    servant.requestFirstPageLoad();
}

int EpisodeListModel::rowCount(const QModelIndex &parent) const {
    return servant.rowCount(parent);
}

QVariant EpisodeListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (!servant.containsItemWithGlobalIndex(index.row())) {
        return QString();
    }
    const auto& episode = servant.getItemByGlobalIndex(index.row());
    return getAttributeOf(episode, role);
}

QVariant EpisodeListModel::getAttributeOf(const core::Episode &episode, int role) const {
    if (role == Role::TITLE) {
        return QString::fromStdString(episode.getName());
    } else if (role == Role::SUBTITLE) {
        return QString::fromStdString(episode.getVideoUrl());
    } else if (role == Role::ICON) {
        return episode.getLastWatchDate() ? "visibility" : "placeholder";
    } else {
        throw InvalidListRole(role);
    }
}

}