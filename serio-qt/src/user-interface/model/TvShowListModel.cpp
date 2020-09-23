#include "TvShowListModel.h"
#include <stdexcept>

serio::qt::TvShowListModel::TvShowListModel(unsigned int pageSize, unsigned int pageCountLimit)
    : servant(pageSize, pageCountLimit, *this) {}

void serio::qt::TvShowListModel::loadPage(const core::ListPage<core::TvShow>& page) {
    servant.loadPage(page);
}

int serio::qt::TvShowListModel::rowCount(const QModelIndex &parent) const {
    return servant.rowCount(parent);
}

int serio::qt::TvShowListModel::columnCount(const QModelIndex &parent) const {
    return roleToName.count();
}

QHash<int, QByteArray> serio::qt::TvShowListModel::roleNames() const {
    return roleToName;
}

QVariant serio::qt::TvShowListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (!servant.containsItemWIthGlobalIndex(index.row())) {
        return QString();
    }
    const core::TvShow& tvShow = servant.getItemByGlobalIndex(index.row());
    return getAttributeOf(tvShow, role);
}

QVariant serio::qt::TvShowListModel::getAttributeOf(const serio::core::TvShow &tvShow, int role) const {
    if (role == Role::NAME) {
        return QString::fromStdString(tvShow.getName());
    } else if (role == Role::THUMBNAIL_URL) {
        return QString::fromStdString(tvShow.getThumbnailUrl());
    } else if (role == Role::LAST_WATCH_DATE) {
        std::optional<core::LastWatchDate> lastWatchDate = tvShow.getLastWatchDate();
        return lastWatchDate ? QString::fromStdString(lastWatchDate->toString()) : QVariant();
    } else {
        throw std::invalid_argument("Invalid role " + std::to_string(role));
    }
}
