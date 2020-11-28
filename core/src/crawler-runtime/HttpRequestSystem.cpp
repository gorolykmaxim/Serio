#include "HttpRequestSystem.h"

namespace serio {
HttpRequestSystem::HttpRequestSystem(std::vector<Crawler> &crawlers, std::vector<CrawlerExecution> &executions,
                                     CrawlerHttpClient& httpClient)
    : executions(executions), httpClient(httpClient) {
    cacheTtls.reserve(crawlers.size());
    for (auto& crawler: crawlers) {
        crawler.code = "function httpRequests(requests) {"
                       "  if (requests.length === undefined) die('httpRequests() argument should be an array of requests');"
                       "  for (let i = 0; i < requests.length; i++) {"
                       "    let request = requests[i];"
                       "    if (typeof request.url !== 'string') die('Each http request should have \'url\' string specified');"
                       "    if (request.method && typeof request.method !== 'string') die('Http request \'method\' must be a string when specified');"
                       "    if (request.body && typeof request.body !== 'string') die('Http request \'body\' must be a string when specified');"
                       "    let headers = request.headers;"
                       "    if (headers) {"
                       "      let fields = [];"
                       "      for (let header in headers) {"
                       "        fields.push({name: header, value: headers[header]});"
                       "      }"
                       "      request.headers = fields;"
                       "    }"
                       "  }"
                       "  _buffer=requests;"
                       "  _waiting='httpRequests';"
                       "  return _buffer;"
                       "} "
                       + crawler.code;
        cacheTtls.emplace_back(crawler.networkCacheTtl);
    }
}

void HttpRequestSystem::update() {
    uint32_t finishedExecutions = 0;
    sendPendingRequests(finishedExecutions);
    waitForResponsesIfNothingElseToDo(finishedExecutions);
}

void HttpRequestSystem::sendPendingRequests(uint32_t &finishedExecutions) {
    for (auto i = 0; i < executions.size(); i++) {
        auto& execution = executions[i];
        const auto isWaiting = execution.isWaiting("httpRequests");
        const auto hasActiveRequests = executionHandleToResponses.find(i) != executionHandleToResponses.cend();
        if (isWaiting && !hasActiveRequests) {
            sendPendingExecutionRequests(execution, i);
        } else if (execution.isDone()) {
            finishedExecutions++;
        }
    }
}

void HttpRequestSystem::waitForResponsesIfNothingElseToDo(uint32_t finishedExecutions) {
    bool allActiveExecutionsWaitingForResponses = finishedExecutions + executionHandleToResponses.size() == executions.size();
    if (allActiveExecutionsWaitingForResponses) {
        for (auto& handleWithResponses: executionHandleToResponses) {
            try {
                deliverResponsesToExecution(handleWithResponses.first, handleWithResponses.second);
            } catch (std::runtime_error& e) {
                auto& execution = executions[handleWithResponses.first];
                execution.fail(e);
            }
        }
        executionHandleToResponses.clear();
    }
}

void HttpRequestSystem::sendPendingExecutionRequests(CrawlerExecution& execution, uint32_t executionHandle) {
    const auto requests = execution.readSharedBuffer();
    const auto requestCount = requests.size();
    std::vector<std::future<std::string>> responses;
    responses.reserve(requestCount);
    for (auto i = 0; i < requestCount; i++) {
        responses.push_back(sendRequest(requests.get(i), cacheTtls[executionHandle]));
    }
    executionHandleToResponses[executionHandle] = std::move(responses);
}

std::future<std::string> HttpRequestSystem::sendRequest(JsObject request, std::chrono::milliseconds cacheTtl) {
    auto url = request.get("url");
    HttpRequest httpRequest{static_cast<std::string>(url)};
    auto method = request.get("method");
    if (!method.isNullOrUndefined()) {
        httpRequest.method = static_cast<std::string>(method);
    }
    auto headers = request.get("headers");
    if (!headers.isNullOrUndefined()) {
        const auto headersSize = headers.size();
        for (auto i = 0; i < headersSize; i++) {
            auto header = headers.get(i);
            httpRequest.headers.emplace(header.get("name"), header.get("value"));
        }
    }
    auto body = request.get("body");
    if (!body.isNullOrUndefined()) {
        httpRequest.body = static_cast<std::string>(body);
    }
    return httpClient.sendRequest(httpRequest, cacheTtl);
}

void HttpRequestSystem::deliverResponsesToExecution(uint32_t executionHandle,
                                                    std::vector<std::future<std::string>>& responses) {
    auto& execution = executions[executionHandle];
    auto context = execution.getContext();
    std::vector<JsObject> results;
    results.reserve(responses.size());
    for (auto& response: responses) {
        results.emplace_back(context, response.get());
    }
    execution.writeSharedBuffer(JsObject(context, results));
}
}