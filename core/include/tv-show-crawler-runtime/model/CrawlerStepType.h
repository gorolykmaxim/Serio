#ifndef SERIO_CRAWLERSTEPTYPE_H
#define SERIO_CRAWLERSTEPTYPE_H

#include <string>
#include <vector>

namespace serio::core {

class CrawlerStepType {
public:
    explicit CrawlerStepType(std::string name, std::string description, std::vector<std::string> mandatoryProperties = {});
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getDescription() const;
    [[nodiscard]] std::vector<std::string> getMandatoryProperties() const;
    bool operator==(const CrawlerStepType &rhs) const;
    bool operator!=(const CrawlerStepType &rhs) const;
private:
    std::string name;
    std::string description;
    std::vector<std::string> mandatoryProperties;
};

}

#endif //SERIO_CRAWLERSTEPTYPE_H
