#ifndef SERIO_FETCHSTEP_H
#define SERIO_FETCHSTEP_H

#include <http-client/HttpClient.h>
#include <tv-show-crawler-runtime/model/CrawlerStep.h>
#include <vector>

namespace serio::core {

std::vector<std::string> executeFetchStep(const CrawlerStep& step, const std::vector<std::string>& previousStepResults,
                                          HttpClient& httpClient);

}

#endif //SERIO_FETCHSTEP_H
