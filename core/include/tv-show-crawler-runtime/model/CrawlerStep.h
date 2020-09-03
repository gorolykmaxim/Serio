#ifndef SERIO_CRAWLERSTEP_H
#define SERIO_CRAWLERSTEP_H

#include <string>
#include <optional>
#include <map>

namespace serio::core {

class CrawlerStep {
public:
    explicit CrawlerStep(std::string type, std::map<std::string, std::string> properties = {});
    [[nodiscard]] std::string getType() const;
    [[nodiscard]] std::optional<std::string> getProperty(const std::string &propertyName) const;
    [[nodiscard]] std::string getPropertyOrFail(const std::string &propertyName) const;
    [[nodiscard]] std::map<std::string, std::string> getProperties() const;
    bool operator==(const CrawlerStep &rhs) const;
    bool operator!=(const CrawlerStep &rhs) const;
private:
    std::string type;
    std::map<std::string, std::string> properties;
};

class CrawlerStepTypeError: public std::invalid_argument {
public:
    explicit CrawlerStepTypeError(const std::string& stepType);
};

class CrawlerStepPropertyMissingError : public std::invalid_argument {
public:
    CrawlerStepPropertyMissingError(const std::string &stepType, const std::string& missingProperty);
};

class CrawlerStepExecutionError : public std::runtime_error {
public:
    CrawlerStepExecutionError(unsigned int stepNumber, const std::exception& error);
};

}

#endif //SERIO_CRAWLERSTEP_H
