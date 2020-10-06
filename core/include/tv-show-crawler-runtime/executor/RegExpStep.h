#ifndef SERIO_REGEXPSTEP_H
#define SERIO_REGEXPSTEP_H

#include <tv-show-crawler-runtime/model/CrawlerStep.h>
#include <vector>

namespace serio::core {

std::vector<std::string> executeRegExpStep(const CrawlerStep &step, const std::vector<std::string> &previousStepResults);

}

#endif //SERIO_REGEXPSTEP_H
