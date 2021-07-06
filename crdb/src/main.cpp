#include <SQLiteCpp/Database.h>
#include <http.h>
#include <crawler.h>
#include <fstream>
#include <cxxopts.hpp>

enum crdb_error {
    wrong_args, unknown, ok
};

static cxxopts::Options create_options() {
    cxxopts::Options options("crdb", "crdb (crawler runtime debugger) - tool to run "
                                     "js scripts as crawlers using serio's crawler runtime");
    options.add_options()
            ("c,cache-ttl", "Amount of time in milliseconds to cache HTTP responses for", cxxopts::value<long>()->default_value("3600000"))
            ("u,user-agent", "User-Agent header value to be used when sending HTTP requests", cxxopts::value<std::string>()->default_value("crdb"))
            ("a,args", "JSON array of arguments to pass to the crawler", cxxopts::value<std::string>()->default_value("[]"))
            ("f,file", "File containing js script to run", cxxopts::value<std::string>())
            ("h,help", "Display help");
    return options;
}

static bool read_script(const std::string& script_path, std::string& script) {
    std::ifstream file(script_path);
    if (!file) {
        return false;
    }
    file >> std::noskipws;
    script = std::string((std::istream_iterator<char>(file)), std::istream_iterator<char>());
    return true;
}

static crdb_error read_args(int argc, char** argv, cxxopts::Options& options, std::vector<crawler>& crawlers_to_execute,
                            std::vector<std::string>& user_agents, id_seed& seed, std::string& error) {
    try {
        const auto result = options.parse(argc, argv);
        if (result.count("help")) {
            return wrong_args;
        }
        crawler crawler{create_id(seed)};
        const auto args = result["args"].as<std::string>();
        crawler.args = std::vector<nlohmann::json>(nlohmann::json::parse(args));
        const auto crawler_path = result["file"].as<std::string>();
        if (!read_script(crawler_path, crawler.code)) {
            error = crawler_path + " does not exist";
            return unknown;
        }
        crawler.cache_ttl = std::chrono::milliseconds(result["cache-ttl"].as<long>());
        crawlers_to_execute.push_back(std::move(crawler));
        user_agents.emplace_back(result["user-agent"].as<std::string>());
        return ok;
    } catch (cxxopts::OptionException& e) {
        return wrong_args;
    } catch (std::domain_error& e) {
        return wrong_args;
    } catch (std::exception& e) {
        error = e.what();
        return unknown;
    }
}

int main(int argc, char** argv) {
    id_seed seed;
    queue<task> task_queue;
    SQLite::Database cache_db(":memory:", SQLite::OPEN_READWRITE);
    http_client client{nativeformat::http::createClient("", "")};
    crawler_runtime runtime;
    runtime.trace = true;
    auto options = create_options();
    std::string err_msg;
    const auto error = read_args(argc, argv, options, runtime.crawlers, client.user_agents, seed, err_msg);
    if (error == wrong_args) {
        std::cout << options.help() << std::endl;
        return 1;
    } else if (error == unknown) {
        std::cerr << err_msg << std::endl;
        return 1;
    }
    init_http_client_cache(cache_db);
    task_queue.enqueue({init_task});
    std::vector<http_response> ress;
    do {
        const auto task = task_queue.dequeue();
        read_http_responses(task, client.response_queue, ress);
        execute_crawlers(runtime, ress, client.requests_to_send, seed);
        send_http_requests(client, cache_db, task_queue, seed);
    } while (!runtime.pending_execs.empty());
    display_profiler_statistics(runtime.profiler_statistics);
    for (const auto& result: runtime.results) {
        std::cout << result.data.dump(2) << std::endl;
    }
    return 0;
}