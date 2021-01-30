#include <crawler-runtime/CrawlerRuntime.h>
#include "CrawlerExecution.h"
#include "RegExpSystem.h"
#include "ResultFetchSystem.h"
#include "CrawlerExecutionSystem.h"
#include "CrawlerArgumentSystem.h"
#include "HttpRequestSystem.h"
#include "LoggingSystem.h"
#include "ProfilerSystem.h"

namespace serio {
CrawlerRuntime::CrawlerRuntime(HttpClient &httpClient, Config& config, bool trace)
    : httpClient(httpClient), config(config), trace(trace) {}

static void executeWithoutTrace(CrawlerExecutionSystem& crawlerExecutionSystem, RegExpSystem& regExpSystem,
                                HttpRequestSystem& httpRequestSystem) {
    while (!crawlerExecutionSystem.isFinished()) {
        crawlerExecutionSystem.update();
        regExpSystem.update();
        httpRequestSystem.update();
    }
}

static void executeWithTrace(std::vector<serio::CrawlerExecution>& executions,
                             CrawlerExecutionSystem& crawlerExecutionSystem, RegExpSystem& regExpSystem,
                             HttpRequestSystem& httpRequestSystem) {
    LoggingSystem loggingSystem(executions);
    ProfilerSystem profilerSystem;
    while (!crawlerExecutionSystem.isFinished()) {
        profilerSystem.update("CrawlerExecutionSystem");
        crawlerExecutionSystem.update();
        profilerSystem.update("LoggingSystem");
        loggingSystem.update();
        profilerSystem.update("RegExpSystem");
        regExpSystem.update();
        profilerSystem.update("HttpRequestSystem");
        httpRequestSystem.update();
    }
    profilerSystem.displayResults();
}

std::vector<nlohmann::json> CrawlerRuntime::executeCrawlers(std::vector<Crawler> crawlers) {
    std::vector<serio::CrawlerExecution> executions;
    RegExpSystem regExpSystem(crawlers, executions);
    HttpRequestSystem httpRequestSystem(crawlers, executions, httpClient, config);
    ResultFetchSystem resultFetchSystem(crawlers, executions);
    CrawlerExecutionSystem crawlerExecutionSystem(crawlers, executions);
    CrawlerArgumentSystem crawlerArgumentSystem(crawlers, executions);
    if (trace) {
        executeWithTrace(executions, crawlerExecutionSystem, regExpSystem, httpRequestSystem);
    } else {
        executeWithoutTrace(crawlerExecutionSystem, regExpSystem, httpRequestSystem);
    }
    return resultFetchSystem.fetch();
}
}