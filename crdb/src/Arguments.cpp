#include "Arguments.h"
#include <iostream>
#include <fstream>
#include <cxxopts.hpp>

namespace crdb {
static void displayHelpAndExit(cxxopts::Options& options) {
    std::cout << options.help() << std::endl;
    std::exit(1);
}

void readArguments(int argc, char** argv, bool& trace, std::string& script, nlohmann::json& crawlerArgs,
                   std::string& userAgent, std::chrono::milliseconds& cacheTtl) {
    cxxopts::Options options("crdb", "crdb (crawler runtime debugger) - tool to run "
                                     "js scripts as crawlers using serio's crawler runtime");
    options.add_options()
            ("c,cache-ttl", "Amount of time in milliseconds to cache HTTP responses for", cxxopts::value<long>()->default_value("3600000"))
            ("u,user-agent", "User-Agent header value to be used when sending HTTP requests", cxxopts::value<std::string>()->default_value("crdb"))
            ("a,args", "JSON array of arguments to pass to the crawler", cxxopts::value<std::string>()->default_value("[]"))
            ("f,file", "File containing js script to run", cxxopts::value<std::string>())
            ("t,trace", "Enable crawler logging and profiling systems", cxxopts::value<bool>()->default_value("false"))
            ("h,help", "Display help");
    try {
        const auto result = options.parse(argc, argv);
        if (result.count("help")) {
            displayHelpAndExit(options);
        }
        trace = result["trace"].as<bool>();
        const auto args = result["args"].as<std::string>();
        crawlerArgs = nlohmann::json::parse(args);
        const auto crawlerPath = result["file"].as<std::string>();
        userAgent = result["user-agent"].as<std::string>();
        cacheTtl = std::chrono::milliseconds(result["cache-ttl"].as<long>());

        std::ifstream file(crawlerPath);
        file >> std::noskipws;
        script = std::string((std::istream_iterator<char>(file)), std::istream_iterator<char>());
    } catch (std::exception& e) {
        displayHelpAndExit(options);
    }
}
}