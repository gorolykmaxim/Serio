#include "AbstractListModel.h"

serio::qt::AbstractListModel::AbstractListModel(QHash<int, QByteArray> roleToName)
        : QAbstractListModel(), roleToName(std::move(roleToName)) {}

QHash<int, QByteArray> serio::qt::AbstractListModel::roleNames() const {
    return roleToName;
}

int serio::qt::AbstractListModel::columnCount(const QModelIndex &parent) const {
    return roleToName.count();
}

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
