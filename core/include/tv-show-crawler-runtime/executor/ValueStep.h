#ifndef SERIO_VALUESTEP_H
#define SERIO_VALUESTEP_H

#include <tv-show-crawler-runtime/model/CrawlerStep.h>
#include <vector>

namespace serio::core {

std::vector<std::string> executeValueStep(const CrawlerStep &step, const std::vector<std::string>& previousStepResults);

}


#endif //SERIO_VALUESTEP_H
