#include "tv-show-crawler-runtime/executor/FetchStep.h"

namespace serio::core {

std::vector<std::string> executeFetchStep(const CrawlerStep &step, const std::vector<std::string> &previousStepResults,
                                          HttpClient &httpClient) {
    return httpClient.fetchContentFromLinks(previousStepResults).get();
}

}