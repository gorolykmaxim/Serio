#include <caching-http-client/HttpResponse.h>

#include <utility>

namespace serio {
HttpResponse::HttpResponse(const std::string &response) : get([response] () {return response; }) {}

HttpResponse::HttpResponse(std::function<std::string()>  get) : get(std::move(get)) {}
}