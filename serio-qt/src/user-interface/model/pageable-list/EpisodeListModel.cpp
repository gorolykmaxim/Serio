#include "EpisodeListModel.h"
#include "InvalidListRole.h"

serio::qt::EpisodeListModel::EpisodeListModel(unsigned int pageSize, unsigned int pageCountLimit)
    : AbstractListModel({
        {Role::NAME, "name"},
        {Role::VIDEO_URL, "videoUrl"},
        {Role::LAST_WATCH_DATE, "lastWatchDate"}
    }), servant(pageSize, pageCountLimit, *this) {}

void serio::qt::EpisodeListModel::loadPage(const serio::core::ListPage<serio::core::Episode> &page) {
    servant.loadPage(page);
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
    if (role == Role::NAME) {
        return QString::fromStdString(episode.getName());
    } else if (role == Role::VIDEO_URL) {
        return QString::fromStdString(episode.getVideoUrl());
    } else if (role == Role::LAST_WATCH_DATE) {
        std::optional<serio::core::LastWatchDate> lastWatchDate = episode.getLastWatchDate();
        return lastWatchDate ? QString::fromStdString(episode.getLastWatchDate()->toString()) : QVariant();
    } else {
        throw serio::qt::InvalidListRole(role);
    }
}
