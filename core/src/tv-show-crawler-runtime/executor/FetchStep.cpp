#include "tv-show-crawler-runtime/executor/FetchStep.h"

std::vector<std::string> serio::core::executeFetchStep(const serio::core::CrawlerStep &step,
                                                       const std::vector<std::string> &previousStepResults,
                                                       serio::core::HttpClient &httpClient) {
    return httpClient.fetchContentFromLinks(previousStepResults).get();
}
