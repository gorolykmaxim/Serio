#ifndef SERIO_LISTMODEL_H
#define SERIO_LISTMODEL_H

#include <QList>

namespace serio::qt {

template <typename T>
class ListModel: public QList<T> {
public:
    void clearAndDelete() {
        if (!this->isEmpty()) {
            qDeleteAll(*this);
            this->clear();
        }
    }
    virtual ~ListModel() {
        clearAndDelete();
    }
};

}

#endif //SERIO_LISTMODEL_H
