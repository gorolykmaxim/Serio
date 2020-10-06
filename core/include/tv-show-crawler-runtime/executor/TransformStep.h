#ifndef SERIO_TRANSFORMSTEP_H
#define SERIO_TRANSFORMSTEP_H

#include <tv-show-crawler-runtime/model/CrawlerStep.h>
#include <vector>

namespace serio::core {

std::vector<std::string> executeTransformStep(const CrawlerStep &step, const std::vector<std::string>& previousStepResults);

}

#endif //SERIO_TRANSFORMSTEP_H
