#ifndef SERIO_CRAWLERSTEP_H
#define SERIO_CRAWLERSTEP_H

#include <string>
#include <optional>
#include <map>

namespace serio::core {

class CrawlerStep {
public:
    CrawlerStep(std::string type, std::map<std::string, std::string> properties = {});
    std::string getType() const;
    std::optional<std::string> getProperty(const std::string &propertyName) const;
    std::string getPropertyOrFail(const std::string &propertyName) const;
    std::map<std::string, std::string> getProperties() const;
    bool operator==(const CrawlerStep &rhs) const;
    bool operator!=(const CrawlerStep &rhs) const;
private:
    std::string type;
    std::map<std::string, std::string> properties;
};

class CrawlerStepTypeError: public std::invalid_argument {
public:
    CrawlerStepTypeError(const std::string& stepType);
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
