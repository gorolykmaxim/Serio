#include <config/HttpClientConfig.h>

namespace serio {
bool HttpClientConfig::operator==(const HttpClientConfig &rhs) const {
    return userAgents == rhs.userAgents;
}

bool HttpClientConfig::operator!=(const HttpClientConfig &rhs) const {
    return !(rhs == *this);
}
}