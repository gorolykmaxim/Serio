#include "AbstractListModel.h"

namespace serio::qt {

AbstractListModel::AbstractListModel(QHash<int, QByteArray> roleToName)
    : QAbstractListModel(), roleToName(std::move(roleToName)) {}

QHash<int, QByteArray> AbstractListModel::roleNames() const {
    return roleToName;
}

int AbstractListModel::columnCount(const QModelIndex &parent) const {
    return roleToName.count();
}

QModelIndex AbstractListModel::createIndex(unsigned int i) const {
    return index(i);
}

void AbstractListModel::insertRows(const QModelIndex &parent, int first, int last) {
    beginInsertRows(parent, first, last);
    endInsertRows();
}

void AbstractListModel::removeRows(const QModelIndex &parent, int first, int last) {
    beginRemoveRows(parent, first, last);
    endRemoveRows();
}

}