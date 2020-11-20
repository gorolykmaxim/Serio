#ifndef SERIO_CACHEMOCK_H
#define SERIO_CACHEMOCK_H

#include <gmock/gmock.h>
#include <cache/Cache.h>
#include "DatabaseStub.h"

namespace mocks {
class CacheMock : public serio::Cache {
public:
    CacheMock() : serio::Cache(mocks::Database::STUB) {}
    MOCK_METHOD(void, put, (const std::string&, const std::string&, const std::chrono::milliseconds&), (override));
    MOCK_METHOD((std::optional<std::string>), get, (const std::string&), (override));
    MOCK_METHOD(long, size, (), (override));
};
}

#endif //SERIO_CACHEMOCK_H
