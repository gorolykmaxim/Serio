#include <iostream>
#include <crawler-runtime/CrawlerRuntime.h>
#include "HardcodedConfig.h"
#include "Arguments.h"

int main(int argc, char** argv) {
    bool trace;
    std::string userAgent;
    std::string script;
    nlohmann::json crawlerArgs;
    std::chrono::milliseconds cacheTtl;
    crdb::readArguments(argc, argv, trace, script, crawlerArgs, userAgent, cacheTtl);

    SQLite::Database database(":memory:", SQLite::OPEN_READWRITE);
    serio::Cache cache(database);
    const auto client = nativeformat::http::createClient("", "");
    serio::HttpClient httpClient(*client, cache);
    crdb::HardcodedConfig config(userAgent, database, httpClient);
    serio::CrawlerRuntime runtime(httpClient, config, trace);

    serio::Crawler crawler{script, cacheTtl, crawlerArgs};
    const auto results = runtime.executeCrawlers({crawler});
    if (!results.empty()) {
        std::cout << results[0].dump(2) << std::endl;
        return 0;
    } else {
        std::cerr << "Crawler execution failed. Use -t option to retry and figure out why." << std::endl;
        return 1;
    }
}