#ifndef SERIO_ARGUMENTS_H
#define SERIO_ARGUMENTS_H

#include <string>
#include <nlohmann/json.hpp>

namespace crdb {
void readArguments(int argc, char** argv, bool& trace, std::string& script, nlohmann::json& crawlerArgs,
                   std::string& userAgent, std::chrono::milliseconds& cacheTtl);
}

#endif //SERIO_ARGUMENTS_H
