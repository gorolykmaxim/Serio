#include "http.h"
#include <sstream>
#include <iconvlite.h>
#include <algorithm>

void init_http_client_cache(SQLite::Database &database) {
    database.exec("CREATE TABLE IF NOT EXISTS CACHE_ENTRY("
                  "KEY TEXT PRIMARY KEY,"
                  "VALUE TEXT,"
                  "EXPIRATION_DATE BIGINT)");
}

static std::optional<std::string> get_from_cache(SQLite::Database &database, const std::string &key, bool include_expired = false) {
    std::stringstream query;
    query << "SELECT VALUE FROM CACHE_ENTRY WHERE KEY=?";
    if (!include_expired) {
        query << " AND EXPIRATION_DATE>?";
    }
    SQLite::Statement select(database, query.str());
    select.bind(1, key);
    if (!include_expired) {
        select.bind(2, std::chrono::system_clock::now().time_since_epoch().count());
    }
    return select.executeStep() ? select.getColumn(0).getString() : std::optional<std::string>();
}

static void put_in_cache(SQLite::Database &database, const std::string &key, const std::string &value, std::chrono::milliseconds duration) {
    SQLite::Statement insert(database, "INSERT OR REPLACE INTO CACHE_ENTRY VALUES(?, ?, ?)");
    insert.bind(1, key);
    insert.bind(2, value);
    insert.bind(3, (std::chrono::system_clock::now() + duration).time_since_epoch().count());
    insert.exec();
}

static std::string make_cache_key_for(const http_request& req) {
    std::stringstream req_headers;
    auto appended_first_header = false;
    for (const auto& entry: req.headers) {
        if (appended_first_header) {
            req_headers << ",";
        }
        req_headers << entry.first << "=" << entry.second;
        appended_first_header = true;
    }
    return  "http::request{url=" + req.url + ",method=" + req.method + ",body=" + req.body + ",headers={" + req_headers.str() + "}}";
}

static std::shared_ptr<nativeformat::http::Request> make_nf_request_for(const http_request& req, const std::vector<std::string>& user_agents) {
    auto headers = req.headers;
    if (headers.find(USER_AGENT_HTTP_HEADER) == headers.cend() && !user_agents.empty()) {
        headers[USER_AGENT_HTTP_HEADER] = user_agents[std::rand() % user_agents.size()];
    }
    auto nf_req = nativeformat::http::createRequest(req.url, headers);
    if (!req.method.empty()) {
        nf_req->setMethod(req.method);
    }
    nf_req->setData(reinterpret_cast<const unsigned char*>(req.body.c_str()), req.body.size());
    return nf_req;
}

static std::string read_body_from(const std::shared_ptr<nativeformat::http::Response>& res) {
    size_t size;
    const auto raw_data = reinterpret_cast<const char*>(res->data(size));
    if (size == 0) {
        return "";
    }
    std::string data(raw_data, size);
    auto content_type = res->operator[](CONTENT_TYPE_HTTP_HEADER);
    std::transform(content_type.begin(), content_type.end(), content_type.begin(), [](unsigned char c) { return std::tolower(c); });
    return content_type.rfind("windows-1251") != std::string::npos ? iconvlite::cp2utf(data) : data;
}

static void send_response(queue<http_response>& response_queue, queue<task>& task_queue, const http_response& res) {
    response_queue.enqueue(res);
    task_queue.enqueue({process_http_response});
}

void send_http_requests(http_client &client, SQLite::Database &database, queue<task>& task_queue) {
    for (const auto& req: client.requests_to_send) {
        const auto req_cache_key = make_cache_key_for(req);
        const auto cached_res = get_from_cache(database, req_cache_key);
        if (cached_res) {
            send_response(client.response_queue, task_queue, {req, *cached_res, nativeformat::http::StatusCodeOK});
            continue;
        }
        const auto nf_req = make_nf_request_for(req, client.user_agents);
        client.nf_client->performRequest(nf_req, [&client, &database, &task_queue, req, req_cache_key] (const auto& nf_res) {
            http_response res{req, read_body_from(nf_res), nf_res->statusCode()};
            if (res.code >= nativeformat::http::StatusCodeBadRequest) {
                const auto outdated_cached_res = get_from_cache(database, req_cache_key, true);
                if (outdated_cached_res) {
                    res.body = *outdated_cached_res;
                    res.code = nativeformat::http::StatusCodeOK;
                }
            } else {
                put_in_cache(database, req_cache_key, res.body, req.cache_ttl);
            }
            send_response(client.response_queue, task_queue, res);
        });
    }
    client.requests_to_send.clear();
}

void read_http_responses(const task &task, queue<http_response> &response_queue, std::vector<http_response>& responses) {
    if (task.id != process_http_response) return;
    responses.push_back(response_queue.dequeue());
}

bool http_request::operator==(const http_request &rhs) const {
    return url == rhs.url &&
           method == rhs.method &&
           body == rhs.body &&
           headers == rhs.headers &&
           cache_ttl == rhs.cache_ttl;
}

bool http_request::operator!=(const http_request &rhs) const {
    return !(rhs == *this);
}

bool http_response::operator==(const http_response &rhs) const {
    return request == rhs.request &&
           body == rhs.body &&
           code == rhs.code;
}

bool http_response::operator!=(const http_response &rhs) const {
    return !(rhs == *this);
}