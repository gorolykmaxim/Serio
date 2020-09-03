#include "tv-show-crawler-runtime/executor/FetchStepExecutor.h"

serio::core::FetchStepExecutor::FetchStepExecutor(serio::core::HttpClient &httpClient) : httpClient(httpClient) {}

std::vector<std::string> serio::core::FetchStepExecutor::execute(const serio::core::CrawlerStep &step,
                                                                 const std::vector<std::string> &previousStepResults) const {
    return httpClient.fetchContentFromLinks(previousStepResults).get();
}
