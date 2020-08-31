#include "TvShowListModel.h"

serio::qt::TvShowListModel::TvShowListModel(unsigned int pageSize, unsigned int pageCountLimit)
    : QAbstractListModel(nullptr), pageSize(pageSize), listProxy(pageSize, pageCountLimit) {}

void serio::qt::TvShowListModel::loadPage(const core::ListPage<core::TvShow>& page) {
    extendModelIfNecessary(page);
    shrinkModelIfNecessary(page);
    pageIsLoaded(page);
}

void serio::qt::TvShowListModel::extendModelIfNecessary(const core::ListPage<core::TvShow>& page) {
    if (page.getTotalSize() > listProxy.getTotalSize()) {
        beginInsertRows(QModelIndex(), listProxy.getTotalSize(), page.getLastItemIndex());
        endInsertRows();
    }
}

void serio::qt::TvShowListModel::shrinkModelIfNecessary(const serio::core::ListPage<serio::core::TvShow> &page) {
    if (page.getTotalSize() < listProxy.getTotalSize()) {
        beginRemoveRows(QModelIndex(), page.getTotalSize(), listProxy.getTotalSize() - 1);
        endRemoveRows();
    }
}

void serio::qt::TvShowListModel::pageIsLoaded(const core::ListPage<core::TvShow>& page) {
    loadingOffsets.erase(page.getOffset());
    listProxy.addPage(page);
    emit dataChanged(index(page.getFirstItemIndex()), index(page.getLastItemIndex()));
}

int serio::qt::TvShowListModel::rowCount(const QModelIndex &parent) const {
    return listProxy.getTotalSize();
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
    if (!listProxy.containsItemWithGlobalIndex(index.row())) {
        loadPageWithIndex(index.row());
        return QString();
    }
    const core::TvShow& tvShow = listProxy.getItemByGlobalIndex(index.row());
    return getAttributeOf(tvShow, role);
}

void serio::qt::TvShowListModel::loadPageWithIndex(unsigned int index) const {
    unsigned int offset = listProxy.getOffsetOfPageWithIndex(index);
    if (loadingOffsets.find(offset) == loadingOffsets.cend()) {
        loadingOffsets.insert(offset);
        emit requestPageLoad(offset, pageSize);
    }
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
