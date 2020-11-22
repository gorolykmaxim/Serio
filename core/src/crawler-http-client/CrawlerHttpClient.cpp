#include <crawler-http-client/CrawlerHttpClient.h>
#include <random>

namespace serio {
CrawlerHttpClient::CrawlerHttpClient(Config &config, CachingHttpClient &client) : config(config), client(client) {}

std::future<std::string> CrawlerHttpClient::sendRequest(HttpRequest request, std::chrono::milliseconds cacheTtl) {
    const auto agents = config.getHttpClientConfig().userAgents;
    request.headers["User-Agent"] = agents[std::rand() % agents.size()];
    return client.sendRequest(request, cacheTtl);
}
}