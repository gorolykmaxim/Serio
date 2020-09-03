#ifndef SERIO_TRANSFORMSTEPEXECUTOR_H
#define SERIO_TRANSFORMSTEPEXECUTOR_H

#include "CrawlerStepExecutor.h"

namespace serio::core {

class TransformStepExecutor : public CrawlerStepExecutor {
public:
    [[nodiscard]] std::vector<std::string> execute(const CrawlerStep &step, const std::vector<std::string>& previousStepResults) const override;
};

}

#endif //SERIO_TRANSFORMSTEPEXECUTOR_H
