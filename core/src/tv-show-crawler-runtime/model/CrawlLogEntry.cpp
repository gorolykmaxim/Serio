#include <sstream>
#include "tv-show-crawler-runtime/model/CrawlLogEntry.h"

namespace serio::core {

class SizeConstrainedStringBuilder {
public:
    static std::string join(const std::vector<std::string>& data, unsigned int maxSize) {
        SizeConstrainedStringBuilder stringBuilder(maxSize);
        stringBuilder.write("[");
        for (int i = 0; i < data.size(); i++) {
            if (!stringBuilder.write(data[i])) {
                break;
            }
            if (i != data.size() - 1) {
                stringBuilder.write(", ");
            }
        }
        return stringBuilder.build();
    }
    explicit SizeConstrainedStringBuilder(unsigned int maxSize) : charsAvailable(maxSize == 0 ? 0 : maxSize - 3) {}
    bool write(const std::string& data) {
        if (charsAvailable > data.size()) {
            stream << data;
            charsAvailable -= data.size();
        } else {
            stream << data.substr(0, charsAvailable);
            charsAvailable = 0;
        }
        return charsAvailable > 0;
    }
    std::string build() {
        if (charsAvailable > 0) {
            stream << "]";
        } else {
            stream << "...";
        }
        return stream.str();
    }
private:
    std::stringstream stream;
    unsigned int charsAvailable;
};

}

serio::core::CrawlLogEntry::CrawlLogEntry(std::string text) : text(std::move(text)) {}

serio::core::CrawlLogEntry::CrawlLogEntry(const serio::core::CrawlerStep& executedStep)
    : text("Executing " + executedStep.getType() + " step with properties: '" + executedStep.getPropertiesAsString() + "'") {}

void serio::core::CrawlLogEntry::setStepInputData(const std::vector<std::string>& data, unsigned int maxSize) {
    stepInputData = SizeConstrainedStringBuilder::join(data, maxSize);
}

void serio::core::CrawlLogEntry::setStepOutputData(const std::vector<std::string> &data, unsigned int maxSize) {
    stepOutputData = SizeConstrainedStringBuilder::join(data, maxSize);
}

bool serio::core::CrawlLogEntry::hasData() const {
    return !stepInputData.empty() || !stepOutputData.empty();
}

std::string serio::core::CrawlLogEntry::getText() const {
    return text;
}

std::string serio::core::CrawlLogEntry::getStepInputData() const {
    return stepInputData;
}

std::string serio::core::CrawlLogEntry::getStepOutputData() const {
    return stepOutputData;
}

bool serio::core::CrawlLogEntry::operator==(const serio::core::CrawlLogEntry &rhs) const {
    return text == rhs.text &&
           stepInputData == rhs.stepInputData &&
           stepOutputData == rhs.stepOutputData;
}

bool serio::core::CrawlLogEntry::operator!=(const serio::core::CrawlLogEntry &rhs) const {
    return !(rhs == *this);
}
