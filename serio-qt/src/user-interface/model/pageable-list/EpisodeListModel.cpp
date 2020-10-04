#include "EpisodeListModel.h"
#include "InvalidListRole.h"

serio::qt::EpisodeListModel::EpisodeListModel(unsigned int pageSize, unsigned int pageCountLimit)
    : AbstractListModel({
        {Role::TITLE,"title"},
        {Role::SUBTITLE,"subtitle"},
        {Role::ICON, "icon"}
    }), servant(pageSize, pageCountLimit, *this) {}

void serio::qt::EpisodeListModel::loadPage(const serio::core::ListPage<serio::core::Episode> &page) {
    servant.loadPage(page);
}

void serio::qt::EpisodeListModel::requestFirstPageLoad() const {
    servant.requestFirstPageLoad();
}

int serio::qt::EpisodeListModel::rowCount(const QModelIndex &parent) const {
    return servant.rowCount(parent);
}

QVariant serio::qt::EpisodeListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (!servant.containsItemWithGlobalIndex(index.row())) {
        return QString();
    }
    const serio::core::Episode& episode = servant.getItemByGlobalIndex(index.row());
    return getAttributeOf(episode, role);
}

QVariant serio::qt::EpisodeListModel::getAttributeOf(const serio::core::Episode &episode, int role) const {
    if (role == Role::TITLE) {
        return QString::fromStdString(episode.getName());
    } else if (role == Role::SUBTITLE) {
        return QString::fromStdString(episode.getVideoUrl());
    } else if (role == Role::ICON) {
        return episode.getLastWatchDate() ? "eye" : "placeholder";
    } else {
        throw serio::qt::InvalidListRole(role);
    }
}
