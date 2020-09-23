#ifndef SERIO_ABSTRACTLISTMODELSERVANT_H
#define SERIO_ABSTRACTLISTMODELSERVANT_H

#include <set>
#include <tv-show-storage/BigListProxy.h>
#include <QModelIndex>
#include <utility>
#include "AbstractListModel.h"

namespace serio::qt {

template <typename T>
class AbstractListModelServant {
public:
    AbstractListModelServant(unsigned int pageSize, unsigned int pageCountLimit, AbstractListModel &model)
        : pageSize(pageSize),
          listProxy(pageSize, pageCountLimit),
          model(model) {}
    void loadPage(const core::ListPage<T>& page) {
        extendModelIfNecessary(page);
        shrinkModelIfNecessary(page);
        pageIsLoaded(page);
    }
    int rowCount(const QModelIndex &parent) const {
        return listProxy.getTotalSize();
    }
    bool containsItemWIthGlobalIndex(unsigned int index) const {
        if (!listProxy.containsItemWithGlobalIndex(index)) {
            loadPageWithIndex(index);
            return false;
        } else {
            return true;
        }
    }
    const T& getItemByGlobalIndex(unsigned int index) const {
        return listProxy.getItemByGlobalIndex(index);
    }
private:
    const unsigned int pageSize;
    mutable std::set<unsigned int> loadingOffsets;
    serio::core::BigListProxy<T> listProxy;
    AbstractListModel& model;
    void extendModelIfNecessary(const core::ListPage<T>& page) {
        if (page.getTotalSize() > listProxy.getTotalSize()) {
            model.insertRows(QModelIndex(), listProxy.getTotalSize(), page.getLastItemIndex());
        }
    }
    void shrinkModelIfNecessary(const core::ListPage<T>& page) {
        if (page.getTotalSize() < listProxy.getTotalSize()) {
            model.removeRows(QModelIndex(), page.getTotalSize(), listProxy.getTotalSize() - 1);
        }
    }
    void pageIsLoaded(const core::ListPage<T>& page) {
        loadingOffsets.erase(page.getOffset());
        listProxy.addPage(page);
        emit model.dataChanged(model.createIndex(page.getFirstItemIndex()), model.createIndex(page.getLastItemIndex()));
    }
    void loadPageWithIndex(unsigned int index) const {
        unsigned int offset = listProxy.getOffsetOfPageWithIndex(index);
        if (loadingOffsets.find(offset) == loadingOffsets.cend()) {
            loadingOffsets.insert(offset);
            emit model.requestPageLoad(offset, pageSize);
        }
    }
};

}

#endif //SERIO_ABSTRACTLISTMODELSERVANT_H
