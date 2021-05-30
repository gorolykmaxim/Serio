#ifndef SERIO_CRAWLER_H
#define SERIO_CRAWLER_H

#include <vector>
#include <nlohmann/json.hpp>
#include <mjs.h>
#include <http.h>
#include "id.h"

struct crawler {
    id id;
    std::string code;
    std::vector<nlohmann::json> args;
    std::chrono::milliseconds cache_ttl;
};

struct crawler_execution {
    id id;
    mjs* mjs;
    mjs_val_t* result;
    mjs_execution exec;
};

struct crawler_http_request {
    using id_t = id;
    id_t id;
    mjs* mjs;
    std::vector<id_t> request_ids;
    std::vector<http_response> responses;
};

struct crawler_http_config {
    id id;
    std::chrono::milliseconds cache_ttl;
};

struct crawler_result {
    id id;
    nlohmann::json data;

    bool operator==(const crawler_result &rhs) const;
    bool operator!=(const crawler_result &rhs) const;
};

struct profiler_statistics {
    std::vector<std::chrono::microseconds> execution_times = std::vector<std::chrono::microseconds>(4);
    std::chrono::system_clock::time_point current_time;
    int current_system_id = -1;
};

struct crawler_runtime {
    std::vector<crawler> crawlers;
    std::vector<crawler_execution> pending_execs;
    std::vector<crawler_http_request> pending_http_reqs;
    std::vector<crawler_http_config> http_configs;
    std::vector<crawler_result> results;
    profiler_statistics profiler_statistics;
    bool trace;
};

void execute_crawlers(crawler_runtime &runtime, std::vector<http_response>& incoming,
                      std::vector<http_request> &outgoing, id_seed& id_seed);
void display_profiler_statistics(const profiler_statistics& stats);

#endif //SERIO_CRAWLER_H
