#ifndef SERIO_EPISODE_H
#define SERIO_EPISODE_H

#include <optional>

namespace serio::core {

class Episode {
public:
    Episode(unsigned int id, std::string videoUrl, std::optional<std::string> episodeName = {});
    [[nodiscard]] unsigned int getId() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getVideoUrl() const;
    bool operator==(const Episode &rhs) const;
    bool operator!=(const Episode &rhs) const;
private:
    unsigned int id;
    std::string name;
    std::string videoUrl;
};

}

#endif //SERIO_EPISODE_H
