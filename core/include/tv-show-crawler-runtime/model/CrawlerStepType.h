#ifndef SERIO_CRAWLERSTEPTYPE_H
#define SERIO_CRAWLERSTEPTYPE_H

#include <string>
#include <vector>

namespace serio::core {

class CrawlerStepType {
public:
    CrawlerStepType(std::string name, std::vector<std::string> mandatoryProperties = {});
    std::string getName() const;
    std::vector<std::string> getMandatoryProperties() const;
    bool operator==(const CrawlerStepType &rhs) const;
    bool operator!=(const CrawlerStepType &rhs) const;
private:
    std::string name;
    std::vector<std::string> mandatoryProperties;
};

}

#endif //SERIO_CRAWLERSTEPTYPE_H
