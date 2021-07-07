#ifndef SERIO_HTTP_H
#define SERIO_HTTP_H

#include <NFHTTP/Client.h>
#include <SQLiteCpp/Database.h>
#include <task.h>
#include <queue.h>
#include "id.h"

const std::string USER_AGENT_HTTP_HEADER = "User-Agent";
const std::string CONTENT_TYPE_HTTP_HEADER = "Content-Type";

struct http_request {
    id id;
    std::string url;
    std::string method;
    std::string body;
    std::unordered_map<std::string, std::string> headers;
    std::chrono::milliseconds cache_ttl;

    bool operator==(const http_request &rhs) const;
    bool operator!=(const http_request &rhs) const;
};

struct http_response {
    http_request request;
    std::string body;
    int code;

    bool operator==(const http_response &rhs) const;
    bool operator!=(const http_response &rhs) const;
};

void init_http_client_cache(SQLite::Database& database);
void send_http_requests(nativeformat::http::Client& nf_client,
                        std::vector<http_request>& requests_to_send, queue<http_response>& response_queue,
                        const std::vector<std::string>& user_agents, SQLite::Database& database, queue<task>& task_queue,
                        id_seed& id_seed);
void read_http_responses(const task& task, queue<http_response>& response_queue, std::vector<http_response>& responses);
std::optional<std::string> get_expired_response_from_cache(SQLite::Database& database, const http_request& req);

#endif //SERIO_HTTP_H