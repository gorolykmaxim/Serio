#include "crawler.h"
#include <regex>
#include <iostream>

enum crawler_system {
    js_system, regex_system, http_system, logging_system
};

enum crawler_request_type {
    regex_req, http_req
};

struct crawler_request {
    id id;
    mjs* mjs;
    crawler_request_type type;
    mjs_val_t buffer;
};

struct crawler_error {
    id id;
    std::string message;
};

template <typename T>
static void move_from_to(typename std::vector<T>::iterator& it, std::vector<T>& from, std::vector<T>& to) {
    to.push_back(*it);
    it = from.erase(it);
}

static void set_attribute_of(mjs *mjs, mjs_val_t obj, const std::string &attribute_name, mjs_val_t val) {
    const auto attr = mjs_mk_string(mjs, attribute_name.c_str(), attribute_name.length(), true);
    mjs_set_v(mjs, obj, attr, val);
}

static mjs_val_t get_attribute_of(mjs *mjs, mjs_val_t obj, const std::string &attribute_name) {
    const auto mjs_name = mjs_mk_string(mjs, attribute_name.c_str(), attribute_name.length(), true);
    return mjs_get_v(mjs, obj, mjs_name);
}

static std::string to_string(mjs *mjs, mjs_val_t str) {
    return mjs_get_cstring(mjs, &str);
}

static void append_string_to_array(mjs* mjs, mjs_val_t array, const std::string& str) {
    mjs_array_push(mjs, array, mjs_mk_string(mjs, str.c_str(), str.size(), true));
}

static std::string stringify(mjs* mjs, mjs_val_t mjs_value) {
    std::string value;
    char* value_ptr;
    if (mjs_json_stringify(mjs, mjs_value, nullptr, 0, &value_ptr) == MJS_OK) {
        value = value_ptr;
        delete value_ptr;
    }
    return value;
}

template <typename T>
static typename std::vector<T>::iterator find_by_id(id id, std::vector<T>& arr) {
    return std::find_if(arr.begin(), arr.end(), [id] (const T& t) {return t.id == id;});
}

template <typename T>
static typename std::vector<T>::const_iterator find_by_id(id id, const std::vector<T>& arr) {
    return std::find_if(arr.cbegin(), arr.cend(), [id] (const T& t) {return t.id == id;});
}

static void create_item_error(std::string& error, const std::string& type, int i, const std::string& message) {
    error = type + " #" + std::to_string(i + 1) + " is invalid: " + message;
}

static bool read_optional_string_attribute(mjs* mjs, mjs_val_t obj, const std::string& attribute, std::string& value) {
    const auto mjs_value = get_attribute_of(mjs, obj, attribute);
    if (!mjs_is_undefined(mjs_value)) {
        if (!mjs_is_string(mjs_value)) {
            return false;
        }
        value = to_string(mjs, mjs_value);
    }
    return true;
}

static bool read_string_attribute(mjs* mjs, mjs_val_t obj, const std::string& attribute, std::string& value) {
    const auto mjs_value = get_attribute_of(mjs, obj, attribute);
    if (!mjs_is_string(mjs_value)) {
        return false;
    }
    value = to_string(mjs, mjs_value);
    return true;
}

static void init_arguments(std::vector<crawler> &crawlers, std::vector<crawler_execution> &active,
                           std::vector<crawler_execution>& failed, std::vector<crawler_error>& errors) {
    auto crawler = crawlers.begin();
    auto execution = active.begin();
    while (crawler != crawlers.end()) {
        const auto mjs = execution->mjs;
        const auto mjs_args = mjs_mk_array(mjs);
        nlohmann::json* wrong_arg = nullptr;
        for (auto& arg: crawler->args) {
            if (arg.is_string()) {
                const auto str = arg.get<std::string>();
                mjs_array_push(mjs, mjs_args, mjs_mk_string(mjs, str.c_str(), str.length(), true));
            } else if (arg.is_boolean()) {
                mjs_array_push(mjs, mjs_args, mjs_mk_boolean(mjs, arg.get<bool>()));
            } else if (arg.is_number()) {
                mjs_array_push(mjs, mjs_args, mjs_mk_number(mjs, arg.get<double>()));
            } else {
                wrong_arg = &arg;
                break;
            }
        }
        if (!wrong_arg) {
            set_attribute_of(mjs, mjs_get_global(mjs), "args", mjs_args);
            crawler++;
            execution++;
        } else {
            errors.push_back({crawler->id, "crawler argument '" + wrong_arg->dump() + "' has unsupported type. Supported types: string, boolean, number."});
            move_from_to(execution, active, failed);
            crawler = crawlers.erase(crawler);
        }
    }
}

static void init_executions(std::vector<crawler> &crawlers, std::vector<crawler_execution> &executions) {
    executions.reserve(crawlers.size());
    for (auto& crawler: crawlers) {
        executions.push_back({crawler.id, mjs_create(), new mjs_val_t()});
        crawler.code = "let _waiting = null; let _buffer = null; " + crawler.code + " ";
    }
}

static void start_executions(std::vector<crawler> &crawlers, std::vector<crawler_execution> &active,
                             std::vector<crawler_execution> &failed, std::vector<crawler_error>& errors) {
    auto crawler = crawlers.begin();
    auto execution = active.begin();
    while (crawler != crawlers.end()) {
        const auto mjs = execution->mjs;
        const auto error = mjs_start_execution(mjs, &execution->exec, crawler->code.c_str(), execution->result);
        if (error == MJS_OK) {
            crawler++;
            execution++;
        } else {
            errors.push_back({crawler->id, mjs_strerror(mjs, error)});
            move_from_to(execution, active, failed);
            crawler = crawlers.erase(crawler);
        }
    }
    crawlers.clear();
}

static void process_failed_requests(std::vector<crawler_request>& failed_reqs,
                                    std::vector<crawler_execution> &pending_execs,
                                    std::vector<crawler_execution> &failed_execs) {
    for (const auto& failed_req: failed_reqs) {
        auto pending_exec = find_by_id(failed_req.id, pending_execs);
        move_from_to(pending_exec, pending_execs, failed_execs);
    }
    failed_reqs.clear();
}

static void process_completed_requests(std::vector<crawler_request>& processed_reqs,
                                       std::vector<crawler_execution> &active_execs,
                                       std::vector<crawler_execution> &pending_execs) {
    for (const auto& processed_req: processed_reqs) {
        const auto mjs = processed_req.mjs;
        auto pending_exec = find_by_id(processed_req.id, pending_execs);
        const auto global = mjs_get_global(mjs);
        set_attribute_of(mjs, global, "_waiting", mjs_mk_null());
        set_attribute_of(mjs, global, "_buffer", processed_req.buffer);
        move_from_to(pending_exec, pending_execs, active_execs);
    }
    processed_reqs.clear();
}

static void submit_request_if_needed(std::vector<crawler_execution>::iterator& exec,
                                     std::vector<crawler_request>& pending_reqs,
                                     std::vector<crawler_execution>& active_execs,
                                     std::vector<crawler_execution>& pending_execs) {
    const auto mjs = exec->mjs;
    const auto global = mjs_get_global(mjs);
    const auto mjs_waiting = get_attribute_of(mjs, global, "_waiting");
    if (!mjs_is_undefined(mjs_waiting) && !mjs_is_null(mjs_waiting)) {
        const auto req_type = static_cast<crawler_request_type>(mjs_get_int(mjs, mjs_waiting));
        const auto buffer = get_attribute_of(mjs, global, "_buffer");
        pending_reqs.push_back({exec->id, mjs, req_type, buffer});
        move_from_to(exec, active_execs, pending_execs);
    }
}

static void run_executions(std::vector<crawler_execution> &active_execs, std::vector<crawler_execution> &pending_execs,
                           std::vector<crawler_execution> &failed_execs, std::vector<crawler_execution> &finished_execs,
                           std::vector<crawler_request>& pending_reqs, std::vector<crawler_error>& errors) {
    for (auto it = active_execs.begin(); it != active_execs.end();) {
        const auto error = mjs_execute_step(&it->exec);
        if (error != MJS_OK) {
            errors.push_back({it->id, mjs_strerror(it->mjs, error)});
            move_from_to(it, active_execs, failed_execs);
        } else if (mjs_is_execution_done(&it->exec)) {
            move_from_to(it, active_execs, finished_execs);
        } else {
            submit_request_if_needed(it, pending_reqs, active_execs, pending_execs);
        }
    }
}

static void finalize_executions(std::vector<crawler_execution> &finished) {
    for (auto& execution: finished) {
        mjs_destroy(execution.mjs);
        delete execution.result;
    }
    finished.clear();
}

static void init_regex(std::vector<crawler> &crawlers) {
    for (auto& crawler: crawlers) {
        crawler.code += "function regex(exp, values) { _buffer = [exp, values]; _waiting = " + std::to_string(regex_req) + "; return _buffer; } ";
    }
}

static bool read_regex_request(const crawler_request& req, std::regex& regex, std::vector<std::string>& values, std::string& error) {
    const auto mjs = req.mjs;
    const auto mjs_regex = mjs_array_get(mjs, req.buffer, 0);
    const auto mjs_values = mjs_array_get(mjs, req.buffer, 1);
    if (!mjs_is_string(mjs_regex)) {
        error = "first argument is not a string";
        return false;
    }
    if (!mjs_is_array(mjs_values)) {
        error = "second argument is not an array";
        return false;
    }
    try {
        regex = std::regex(to_string(mjs, mjs_regex));
    } catch (std::exception& e) {
        error = e.what();
        return false;
    }
    const auto values_count = mjs_array_length(mjs, mjs_values);
    values.reserve(values_count);
    for (auto i = 0 ; i < values_count; i++) {
        auto mjs_value = mjs_array_get(mjs, mjs_values, i);
        if (!mjs_is_string(mjs_value)) {
            create_item_error(error, "value", i, "not a string");
            return false;
        }
        values.emplace_back(mjs_get_cstring(mjs, &mjs_value));
    }
    return true;
}

static void search_regex(const std::regex& regex, const std::vector<std::string>& values, mjs* mjs, mjs_val_t results) {
    for (const auto& value: values) {
        const auto start = std::sregex_iterator(value.cbegin(), value.cend(), regex);
        const auto end = std::sregex_iterator();
        for (auto it = start; it != end; it++) {
            if (it->size() > 1) {
                for (auto i = 1; i < it->size(); i++) {
                    append_string_to_array(mjs, results, it->operator[](i).str());
                }
            } else {
                append_string_to_array(mjs, results, it->operator[](0).str());
            }
        }
    }
}

static void process_regex_requests(std::vector<crawler_request>& pending, std::vector<crawler_request>& processed,
                                   std::vector<crawler_request>& failed, std::vector<crawler_error>& errors) {
    for (auto req = pending.begin(); req != pending.end();) {
        if (req->type == regex_req) {
            const auto mjs = req->mjs;
            std::regex regex;
            std::vector<std::string> values;
            std::string error;
            if (!read_regex_request(*req, regex, values, error)) {
                errors.push_back({req->id, "failed to execute regex(): " + error});
                move_from_to(req, pending, failed);
                continue;
            }
            const auto mjs_results = mjs_mk_array(mjs);
            search_regex(regex, values, mjs, mjs_results);
            req->buffer = mjs_results;
            move_from_to(req, pending, processed);
        } else {
            req++;
        }
    }
}

static void init_http(std::vector<crawler>& crawlers, std::vector<crawler_http_config>& http_configs) {
    http_configs.reserve(http_configs.size() + crawlers.size());
    for (auto& crawler: crawlers) {
        crawler.code += "function http(requests) { _buffer = requests; _waiting = " + std::to_string(http_req) + "; return _buffer; } ";
        http_configs.push_back({crawler.id, crawler.cache_ttl});
    }
}

static bool read_http_headers(mjs* mjs, mjs_val_t mjs_req, std::unordered_map<std::string, std::string>& headers, std::string& error) {
    const auto mjs_headers = get_attribute_of(mjs, mjs_req, "headers");
    if (!mjs_is_undefined(mjs_headers)) {
        if (!mjs_is_array(mjs_headers)) {
            error = "'headers' is not an array";
            return false;
        }
        const auto header_count = mjs_array_length(mjs, mjs_headers);
        for (auto j = 0; j < header_count; j++) {
            const auto mjs_header = mjs_array_get(mjs, mjs_headers, j);
            std::string name, value;
            if (!read_string_attribute(mjs, mjs_header, "name", name)) {
                create_item_error(error, "header", j, "'name' is not specified");
                return false;
            }
            if (!read_string_attribute(mjs, mjs_header, "value", value)) {
                create_item_error(error, "header", j, "'value' is not specified");
                return false;
            }
            headers[name] = value;
        }
    }
    return true;
}

static bool read_http_requests(const crawler_request& req, std::vector<http_request>& outgoing,
                               const std::vector<crawler_http_config>& http_configs,
                               std::vector<crawler_http_request>& pending_http_reqs, id_seed& id_seed,
                               std::string& error) {
    const auto mjs = req.mjs;
    const auto config = find_by_id(req.id, http_configs);
    const auto req_count = mjs_array_length(mjs, req.buffer);
    std::vector<http_request> reqs_to_send;
    std::vector<id> req_ids;
    reqs_to_send.reserve(req_count);
    req_ids.reserve(req_count);
    for (auto i = 0; i < req_count; i++) {
        const auto id = create_id(id_seed);
        req_ids.push_back(id);
        http_request http_req{id};
        http_req.cache_ttl = config->cache_ttl;
        const auto mjs_req = mjs_array_get(mjs, req.buffer, i);
        const auto mjs_url = get_attribute_of(mjs, mjs_req, "url");
        if (!mjs_is_string(mjs_url)) {
            create_item_error(error, "request", i, "'url' is not a string");
            return false;
        }
        http_req.url = to_string(mjs, mjs_url);
        if (!read_optional_string_attribute(mjs, mjs_req, "method", http_req.method)) {
            create_item_error(error, "request", i, "'method' is not a string");
            return false;
        }
        if (!read_optional_string_attribute(mjs, mjs_req, "body", http_req.body)) {
            create_item_error(error, "request", i, "'body' is not a string");
            return false;
        }
        std::string header_error;
        if (!read_http_headers(mjs, mjs_req, http_req.headers, header_error)) {
            create_item_error(error, "request", i, "headers are invalid: " + header_error);
            return false;
        }
        reqs_to_send.push_back(std::move(http_req));
    }
    pending_http_reqs.push_back({req.id, mjs, std::move(req_ids)});
    outgoing.reserve(outgoing.size() + reqs_to_send.size());
    outgoing.insert(outgoing.end(), reqs_to_send.begin(), reqs_to_send.end());
    return true;
}

static bool is_response_to(const http_response& res, const crawler_http_request& req) {
    return std::find(req.request_ids.cbegin(), req.request_ids.cend(), res.request.id) != req.request_ids.cend();
}

static void assign_http_responses_to_crawler_requests(std::vector<http_response>& responses,
                                                      std::vector<crawler_http_request>& pending_http_reqs) {
    for (auto it = responses.begin(); it != responses.end();) {
        auto pending = std::find_if(pending_http_reqs.begin(), pending_http_reqs.end(),
                                    [it] (const crawler_http_request& r) {return is_response_to(*it, r);});
        if (pending != pending_http_reqs.end()) {
            move_from_to(it, responses, pending->responses);
        } else {
            it++;
        }
    }
}

static void deliver_http_responses_to_executions(std::vector<crawler_http_request>& pending_http_reqs,
                                                 std::vector<crawler_request>& processed_reqs,
                                                 std::vector<crawler_request>& failed_reqs,
                                                 std::vector<crawler_error>& errors) {
    for (auto it = pending_http_reqs.begin(); it != pending_http_reqs.end();) {
        if (it->responses.size() < it->request_ids.size()) {
            it++;
            continue;
        }
        const auto mjs = it->mjs;
        const auto buffer = mjs_mk_array(mjs);
        auto* crawler_reqs = &processed_reqs;
        for (const auto& res: it->responses) {
            if (res.code > 399) {
                crawler_reqs = &failed_reqs;
                errors.push_back({it->id, "http request failed: " + std::to_string(res.code) + " " + res.body});
                break;
            } else {
                const auto body = mjs_mk_string(mjs, res.body.c_str(), res.body.size(), true);
                mjs_array_push(mjs, buffer, body);
            }
        }
        crawler_reqs->push_back({it->id, mjs, http_req, buffer});
        it = pending_http_reqs.erase(it);
    }
}

static void send_http_requests(std::vector<crawler_request>& pending_reqs, std::vector<crawler_request>& failed_reqs,
                               std::vector<http_request>& outgoing, const std::vector<crawler_http_config>& http_configs,
                               std::vector<crawler_http_request>& pending_http_reqs, id_seed& id_seed, std::vector<crawler_error>& errors) {
    for (auto req = pending_reqs.begin(); req != pending_reqs.end();) {
        if (req->type == http_req) {
            std::string error;
            if (read_http_requests(*req, outgoing, http_configs, pending_http_reqs, id_seed, error)) {
                req = pending_reqs.erase(req);
            } else {
                errors.push_back({req->id, "failed to execute http(): " + error});
                move_from_to(req, pending_reqs, failed_reqs);
            }
        } else {
            req++;
        }
    }
}

static void remove_http_configs_of_executions(const std::vector<crawler_execution>& execs,
                                              std::vector<crawler_http_config>& http_configs) {
    for (const auto& exec: execs) {
        auto it = find_by_id(exec.id, http_configs);
        if (it != http_configs.end()) {
            http_configs.erase(it);
        }
    }
}

static void init_result_fetching(std::vector<crawler> &crawlers) {
    for (auto& crawler: crawlers) {
        crawler.code += "JSON.stringify(crawl(args));";
    }
}

static void fetch_results(std::vector<crawler_execution> &finished, std::vector<crawler_result> &results) {
    results.reserve(results.size() + finished.size());
    for (auto& execution: finished) {
        const std::string raw_result = mjs_get_cstring(execution.mjs, execution.result);
        if (!raw_result.empty()) {
            auto result_data = nlohmann::json::parse(raw_result);
            results.push_back({execution.id, std::move(result_data)});
        }
    }
}

static void log(id id, const std::vector<std::string>& messages) {
    std::cerr << "Crawler " << id << " ";
    for (const auto& message: messages) {
        std::cerr << message << std::endl;
    }
    std::cerr << std::endl;
}

static void log_failed_executions(bool trace, const std::vector<crawler_execution>& execs, const std::vector<crawler_error>& errors) {
    if (!trace) return;
    for (const auto& exec: execs) {
        auto error = find_by_id(exec.id, errors);
        log(exec.id, {"Has failed", error->message});
    }
}

static void log_finished_executions(bool trace, const std::vector<crawler_execution>& execs) {
    if (!trace) return;
    for (const auto& exec: execs) {
        log(exec.id, {"Finished executing successfully"});
    }
}

static void log_pending_executions(bool trace, const std::vector<crawler_request>& pending_reqs) {
    if (!trace) return;
    for (const auto& req: pending_reqs) {
        const std::string waiting = req.type == regex_req ? "regex" : "http";
        log(req.id, {
                "Has started waiting",
                "Waiting for: " + waiting,
                "Buffer: " + stringify(req.mjs, req.buffer)
        });
    }
}

static void log_resuming_executions(bool trace, const std::vector<crawler_request>& processed_reqs) {
    if (!trace) return;
    for (const auto& req: processed_reqs) {
        log(req.id, {"Back to running", "Buffer: " + stringify(req.mjs, req.buffer)});
    }
}

static void trace(int system, crawler_runtime& runtime) {
    if (!runtime.trace) return;
    auto& stats = runtime.profiler_statistics;
    const auto now = std::chrono::system_clock::now();
    if (stats.current_system_id >= 0) {
        stats.execution_times[stats.current_system_id] += now - stats.current_time;
    }
    stats.current_system_id = system;
    stats.current_time = now;
}

void execute_crawlers(crawler_runtime &runtime, std::vector<http_response>& incoming,
                      std::vector<http_request> &outgoing, id_seed& id_seed) {
    std::vector<crawler_execution> active_execs, failed_execs, finished_execs;
    std::vector<crawler_request> pending_reqs, processed_reqs, failed_reqs;
    std::vector<crawler_error> errors;
    trace(js_system, runtime);
    init_executions(runtime.crawlers, active_execs);
    init_arguments(runtime.crawlers, active_execs, failed_execs, errors);
    trace(regex_system, runtime);
    init_regex(runtime.crawlers);
    trace(http_system, runtime);
    init_http(runtime.crawlers, runtime.http_configs);
    assign_http_responses_to_crawler_requests(incoming, runtime.pending_http_reqs);
    deliver_http_responses_to_executions(runtime.pending_http_reqs, processed_reqs, failed_reqs, errors);
    trace(js_system, runtime);
    init_result_fetching(runtime.crawlers);
    start_executions(runtime.crawlers, active_execs, failed_execs, errors);
    while (!active_execs.empty() || !processed_reqs.empty() || !failed_reqs.empty()) {
        trace(logging_system, runtime);
        log_resuming_executions(runtime.trace, processed_reqs);
        trace(js_system, runtime);
        process_failed_requests(failed_reqs, runtime.pending_execs, failed_execs);
        process_completed_requests(processed_reqs, active_execs, runtime.pending_execs);
        run_executions(active_execs, runtime.pending_execs, failed_execs, finished_execs, pending_reqs, errors);
        trace(logging_system, runtime);
        log_pending_executions(runtime.trace, pending_reqs);
        trace(regex_system, runtime);
        process_regex_requests(pending_reqs, processed_reqs, failed_reqs, errors);
        trace(http_system, runtime);
        send_http_requests(pending_reqs, failed_reqs, outgoing, runtime.http_configs, runtime.pending_http_reqs, id_seed, errors);
    }
    trace(http_system, runtime);
    remove_http_configs_of_executions(failed_execs, runtime.http_configs);
    remove_http_configs_of_executions(finished_execs, runtime.http_configs);
    trace(logging_system, runtime);
    log_failed_executions(runtime.trace, failed_execs, errors);
    log_finished_executions(runtime.trace, finished_execs);
    trace(js_system, runtime);
    fetch_results(finished_execs, runtime.results);
    finalize_executions(failed_execs);
    finalize_executions(finished_execs);
    trace(!runtime.pending_http_reqs.empty() ? http_system : -1, runtime);
}

void display_profiler_statistics(const profiler_statistics &stats) {
    const auto old_precision = std::cerr.precision();
    std::cerr << std::fixed << std::setprecision(2);
    std::chrono::microseconds total(0);
    for (const auto& time: stats.execution_times) {
        total += time;
    }
    std::cerr << "Profiler results:" << std::endl;
    for (auto i = 0; i < stats.execution_times.size(); i++) {
        std::string system_name;
        if (i == js_system) {
            system_name = "JavaScript system";
        } else if (i == regex_system) {
            system_name = "RegExp system";
        } else if (i == http_system) {
            system_name = "HTTP system";
        } else {
            system_name = "Logging system";
        }
        const auto duration = stats.execution_times[i];
        const auto percentage = total.count() > 0 ? duration * 100.0 / total : 0;
        std::cerr << system_name << ": " << duration.count() / 1000.0 << "ms (" << percentage << "%)" << std::endl;
    }
    std::cerr << "Total duration: " << total.count() / 1000.0 << "ms" << std::endl << std::endl;
    std::cerr << std::defaultfloat << std::setprecision(old_precision);
}

bool crawler_result::operator==(const crawler_result &rhs) const {
    return data == rhs.data;
}

bool crawler_result::operator!=(const crawler_result &rhs) const {
    return !(rhs == *this);
}
