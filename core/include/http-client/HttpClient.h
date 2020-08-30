#ifndef SERIO_HTTPCLIENT_H
#define SERIO_HTTPCLIENT_H

#include <future>

namespace serio::core {

class HttpClient {
public:
    virtual std::future<std::vector<std::string>> fetchContentFromLinks(const std::vector<std::string>& links) = 0;
};

}

#endif //SERIO_HTTPCLIENT_H
