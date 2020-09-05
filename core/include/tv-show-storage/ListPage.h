#ifndef SERIO_LISTPAGE_H
#define SERIO_LISTPAGE_H

#include <vector>

namespace serio::core {

template <typename T>
class ListPage {
public:
    explicit ListPage(unsigned int offset = 0, unsigned int totalSize = 0, std::vector<T> items = {})
            : offset(offset), totalSize(totalSize), items(std::move(items)) {
        if (this->offset + this->items.size() > this->totalSize) {
            throw std::invalid_argument("Page outflows parent list according to specified offset and items");
        }
    }
    [[nodiscard]] unsigned int getOffset() const {
        return offset;
    }
    [[nodiscard]] unsigned int getTotalSize() const {
        return totalSize;
    }
    [[nodiscard]] bool containsItemWithGlobalIndex(unsigned int index) const {
        return index >= offset && index < offset + items.size();
    }
    const T& getItemByGlobalIndex(unsigned int index) const {
        return items.at(index - offset);
    }
    [[nodiscard]] unsigned int getFirstItemIndex() const {
        return offset;
    }
    [[nodiscard]] unsigned int getLastItemIndex() const {
        return offset + items.size() - 1;
    }
    bool operator<(const ListPage &rhs) const {
        return offset < rhs.offset;
    }
    bool operator>(const ListPage &rhs) const {
        return rhs < *this;
    }
    bool operator<=(const ListPage &rhs) const {
        return !(rhs < *this);
    }
    bool operator>=(const ListPage &rhs) const {
        return !(*this < rhs);
    }
private:
    unsigned int offset, totalSize;
    std::vector<T> items{};
};

}

#endif //SERIO_LISTPAGE_H
