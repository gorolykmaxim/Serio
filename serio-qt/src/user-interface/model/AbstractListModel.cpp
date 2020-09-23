#include "AbstractListModel.h"

QModelIndex serio::qt::AbstractListModel::createIndex(unsigned int i) const {
    return index(i);
}

void serio::qt::AbstractListModel::insertRows(const QModelIndex &parent, int first, int last) {
    beginInsertRows(parent, first, last);
    endInsertRows();
}

void serio::qt::AbstractListModel::removeRows(const QModelIndex &parent, int first, int last) {
    beginRemoveRows(parent, first, last);
    endRemoveRows();
}
