#ifndef SERIO_ABSTRACTLISTMODEL_H
#define SERIO_ABSTRACTLISTMODEL_H

#include <QAbstractListModel>

namespace serio::qt {

class AbstractListModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit AbstractListModel(QHash<int, QByteArray> roleToName);
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent) const override;
    [[nodiscard]] QModelIndex createIndex(unsigned int i) const;
    void insertRows(const QModelIndex &parent, int first, int last);
    void removeRows(const QModelIndex &parent, int first, int last);
signals:
    void requestPageLoad(unsigned int offset, unsigned int size) const;
private:
    const QHash<int, QByteArray> roleToName;
};

}

#endif //SERIO_ABSTRACTLISTMODEL_H
