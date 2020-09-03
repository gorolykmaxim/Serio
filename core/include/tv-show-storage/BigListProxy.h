#ifndef SERIO_BIGLISTPROXY_H
#define SERIO_BIGLISTPROXY_H

#include <utility>
#include <algorithm>
#include <string>
#include <list>
#include "ListPage.h"

namespace serio::core {

template <typename T>
class BigListProxy {
public:
    BigListProxy(const unsigned int pageSize, const unsigned int pageCountLimit)
            : totalSize(0), pageSize(pageSize), pageCountLimit(pageCountLimit) {}
    [[nodiscard]] unsigned int getTotalSize() const {
        return totalSize;
    }
    void addPage(serio::core::ListPage<T> page) {
        totalSize = page.getTotalSize();
        pages.push_back(std::move(page));
        if (pages.size() > pageCountLimit) {
            pages.pop_front();
        }
    }
    [[nodiscard]] bool containsItemWithGlobalIndex(unsigned int index) const {
        auto it = std::find_if(pages.cbegin(), pages.cend(), [=] (ListPage<T> item) {return item.containsItemWithGlobalIndex(index);});
        return it != pages.cend();
    }
    const T& getItemByGlobalIndex(unsigned int index) const {
        auto it = std::find_if(pages.cbegin(), pages.cend(), [=] (ListPage<T> item) {return item.containsItemWithGlobalIndex(index);});
        if (it == pages.cend()) {
            throw std::out_of_range(std::to_string(index));
        } else {
            return it->getItemByGlobalIndex(index);
        }
    }
    [[nodiscard]] unsigned int getOffsetOfPageWithIndex(unsigned int index) const {
        return index / pageSize * pageSize;
    }
private:
    unsigned int totalSize;
    const unsigned int pageSize, pageCountLimit;
    std::list<serio::core::ListPage<T>> pages;
};

}

#endif //SERIO_BIGLISTPROXY_H
