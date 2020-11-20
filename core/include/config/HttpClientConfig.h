#ifndef SERIO_HTTPCLIENTCONFIG_H
#define SERIO_HTTPCLIENTCONFIG_H

#include <vector>
#include <string>

namespace serio {
struct HttpClientConfig {
    std::vector<std::string> userAgents;

    bool operator==(const HttpClientConfig &rhs) const;
    bool operator!=(const HttpClientConfig &rhs) const;
};
}

#endif //SERIO_HTTPCLIENTCONFIG_H
