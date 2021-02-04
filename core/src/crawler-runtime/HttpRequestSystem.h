#ifndef SERIO_HTTPREQUESTSYSTEM_H
#define SERIO_HTTPREQUESTSYSTEM_H

#include <vector>
#include <set>
#include <crawler-runtime/CrawlerRuntime.h>
#include "CrawlerExecution.h"

namespace serio {
class HttpRequestSystem {
public:
    HttpRequestSystem(std::vector<Crawler>& crawlers, std::vector<CrawlerExecution>& executions,
                      HttpClient& httpClient, Config& config);
    void update();
private:
    HttpClient& httpClient;
    Config& config;
    std::vector<CrawlerExecution>& executions;
    std::vector<std::chrono::milliseconds> cacheTtls;
    std::unordered_map<uint32_t, std::vector<HttpResponse>> executionHandleToResponses;

    void sendPendingRequests(uint32_t& finishedExecutions);
    void waitForResponsesIfNothingElseToDo(uint32_t finishedExecutions);
    void sendPendingExecutionRequests(CrawlerExecution& execution, uint32_t executionHandle);
    HttpResponse sendRequest(JsObject request, std::chrono::milliseconds cacheTtl);
    void setUserAgentFromConfig(HttpRequest& request);
    void deliverResponsesToExecution(uint32_t executionHandle, std::vector<HttpResponse>& responses);
};
}

#endif //SERIO_HTTPREQUESTSYSTEM_H
