#ifndef SERIO_TEST_DATA_H
#define SERIO_TEST_DATA_H

#include <string>
#include "test-lib.h"

const std::string CONFIG_URL = "https://crawler-config-url.com";
const std::string CONFIG = "{}";
const std::string WRONG_CONFIG_URL = "https://wrong-crawler-config-url.com";
const std::string WRONG_CONFIG = create_long_string("Internal server error", 50);
const std::string BROKEN_CONFIG_URL = "https://broken-crawler-config-url.com";

#endif //SERIO_TEST_DATA_H
