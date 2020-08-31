#ifndef SERIO_TVSHOW_H
#define SERIO_TVSHOW_H

#include <string>
#include <chrono>
#include <optional>
#include "LastWatchDate.h"

namespace serio::core {

class TvShow {
public:
    TvShow(std::string name, std::string thumbnailUrl = "");
    TvShow(std::string name, std::string thumbnailUrl, LastWatchDate lastWatchDate);
    std::string getName() const;
    std::string getThumbnailUrl() const;
    std::optional<LastWatchDate> getLastWatchDate() const;
    bool operator==(const TvShow &rhs) const;
    bool operator!=(const TvShow &rhs) const;
private:
    std::string name;
    std::string thumbnailUrl;
    std::optional<LastWatchDate> lastWatchDate;
};

}

#endif //SERIO_TVSHOW_H
