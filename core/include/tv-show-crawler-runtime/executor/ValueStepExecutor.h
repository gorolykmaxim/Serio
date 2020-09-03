#ifndef SERIO_VALUESTEPEXECUTOR_H
#define SERIO_VALUESTEPEXECUTOR_H

#include "CrawlerStepExecutor.h"

namespace serio::core {

class ValueStepExecutor : public CrawlerStepExecutor {
public:
    [[nodiscard]] std::vector<std::string> execute(const CrawlerStep &step, const std::vector<std::string>& previousStepResults) const override;
};

}


#endif //SERIO_VALUESTEPEXECUTOR_H
