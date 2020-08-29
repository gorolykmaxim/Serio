#ifndef SERIO_TVSHOW_H
#define SERIO_TVSHOW_H

#include <string>
#include <chrono>
#include <memory>
#include "LastWatchDate.h"

namespace serio::core {

class TvShow {
public:
    TvShow(std::string name, std::string thumbnailUrl = "");
    TvShow(std::string name, std::string thumbnailUrl, LastWatchDate lastWatchDate);
    TvShow(const TvShow &other);
    TvShow(TvShow &&other) noexcept;
    std::string getName() const;
    std::string getThumbnailUrl() const;
    LastWatchDate* getLastWatchDate() const;
    TvShow& operator=(TvShow rhs);
    TvShow& operator=(TvShow &&rhs) noexcept;
    bool operator==(const TvShow &rhs) const;
    bool operator!=(const TvShow &rhs) const;
private:
    std::string name;
    std::string thumbnailUrl;
    std::unique_ptr<LastWatchDate> lastWatchDate;
};

}

#endif //SERIO_TVSHOW_H
