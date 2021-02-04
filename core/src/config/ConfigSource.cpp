#include <config/ConfigSource.h>

namespace serio {
serio::ConfigSource::ConfigSource(PersistentProperties &properties, HttpClient& client)
    : properties(properties),
      client(client) {}

void ConfigSource::setUrl(const std::string &url) {
    properties.setProperty(SOURCE_URL_PROPERTY, url);
}

std::optional<std::string> ConfigSource::getUrl() {
    return properties.getProperty(SOURCE_URL_PROPERTY);
}

ConfigStructure ConfigSource::fetchConfig() {
    try {
        const auto url = properties.getProperty(SOURCE_URL_PROPERTY);
        if (!url) {
            throw ConfigSourceNotSpecifiedError();
        }
        HttpRequest request{*url};
        const auto response = client.sendRequest(request, CACHE_TTL).get();
        const auto jsonRoot = nlohmann::json::parse(response);
        return ConfigStructure(jsonRoot);
    } catch (std::runtime_error& e) {
        throw ConfigFetchError(e.what());
    }
}

ConfigSourceNotSpecifiedError::ConfigSourceNotSpecifiedError()
    : std::runtime_error("Source URL to fetch the configuration from has not been specified") {}

ConfigFetchError::ConfigFetchError(const std::string &reason) : std::runtime_error(reason) {}
}
