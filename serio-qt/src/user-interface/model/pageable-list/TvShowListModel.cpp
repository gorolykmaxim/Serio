#include "TvShowListModel.h"
#include "InvalidListRole.h"

serio::qt::TvShowListModel::TvShowListModel(unsigned int pageSize, unsigned int pageCountLimit)
    : AbstractListModel({
        {Role::NAME, "name"},
        {Role::THUMBNAIL_URL, "thumbnailUrl"},
        {Role::LAST_WATCH_DATE, "lastWatchDate"}
    }), servant(pageSize, pageCountLimit, *this) {}

void serio::qt::TvShowListModel::loadPage(const core::ListPage<core::TvShow>& page) {
    servant.loadPage(page);
}

int serio::qt::TvShowListModel::rowCount(const QModelIndex &parent) const {
    return servant.rowCount(parent);
}

QVariant serio::qt::TvShowListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (!servant.containsItemWithGlobalIndex(index.row())) {
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
        return getLastWatchDate(tvShow.getLastWatchDate());
    } else {
        throw serio::qt::InvalidListRole(role);
    }
}

QString serio::qt::TvShowListModel::getLastWatchDate(const std::optional<core::LastWatchDate> &lastWatchDate) const {
    if (lastWatchDate) {
        QString date = QString::fromStdString(lastWatchDate->toString());
        date[0] = date[0].toUpper();
        return date;
    } else {
        return QString();
    }
}
