#ifndef SERIO_HTTPRESPONSE_H
#define SERIO_HTTPRESPONSE_H

#include <string>

namespace serio {
struct HttpResponse {
    explicit HttpResponse(const std::string& response = "");
    explicit HttpResponse(std::function<std::string()> get);
    std::function<std::string()> get;
};
}

#endif //SERIO_HTTPRESPONSE_H
