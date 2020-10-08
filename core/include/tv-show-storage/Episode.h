#ifndef SERIO_EPISODE_H
#define SERIO_EPISODE_H

#include <optional>
#include "LastWatchDate.h"
#include "WatchProgress.h"

namespace serio::core {

class Episode {
public:
    Episode(unsigned int id, std::string videoUrl, std::optional<std::string> episodeName = {},
            std::optional<LastWatchDate> lastWatchDate = {}, WatchProgress watchProgress = WatchProgress());
    [[nodiscard]] unsigned int getId() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getVideoUrl() const;
    [[nodiscard]] std::optional<LastWatchDate> getLastWatchDate() const;
    [[nodiscard]] WatchProgress getWatchProgress() const;
    [[nodiscard]] unsigned int getNextEpisodeId() const;
    bool operator==(const Episode &rhs) const;
    bool operator!=(const Episode &rhs) const;
private:
    unsigned int id;
    std::string name;
    std::string videoUrl;
    std::optional<LastWatchDate> lastWatchDate;
    WatchProgress watchProgress;
};

}

#endif //SERIO_EPISODE_H
