#ifndef SERIO_ABSTRACTLISTMODEL_H
#define SERIO_ABSTRACTLISTMODEL_H

#include <QAbstractListModel>

namespace serio::qt {

class AbstractListModel : public QAbstractListModel {
    Q_OBJECT
public:
    QModelIndex createIndex(unsigned int i) const;
    void insertRows(const QModelIndex &parent, int first, int last);
    void removeRows(const QModelIndex &parent, int first, int last);
signals:
    void requestPageLoad(unsigned int offset, unsigned int size) const;
};

}

#endif //SERIO_ABSTRACTLISTMODEL_H
