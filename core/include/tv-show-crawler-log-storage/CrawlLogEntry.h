#ifndef SERIO_CRAWLLOGENTRY_H
#define SERIO_CRAWLLOGENTRY_H

#include <string>
#include <vector>
#include "tv-show-crawler-runtime/model/CrawlerStep.h"

namespace serio::core {

class CrawlLogEntry {
public:
    explicit CrawlLogEntry(std::string text);
    explicit CrawlLogEntry(const serio::core::CrawlerStep& executedStep);
    CrawlLogEntry(std::string text, std::string stepInputData, std::string stepOutputData);
    void setStepInputData(const std::vector<std::string>& data, unsigned int maxSize);
    void setStepOutputData(const std::vector<std::string>& data, unsigned int maxSize);
    [[nodiscard]] bool hasData() const;
    [[nodiscard]] std::string getText() const;
    [[nodiscard]] std::string getStepInputData() const;
    [[nodiscard]] std::string getStepOutputData() const;
    bool operator==(const CrawlLogEntry &rhs) const;
    bool operator!=(const CrawlLogEntry &rhs) const;
private:
    std::string text;
    std::string stepInputData;
    std::string stepOutputData;
};

}

#endif //SERIO_CRAWLLOGENTRY_H
