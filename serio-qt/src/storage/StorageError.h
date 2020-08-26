#ifndef SERIO_STORAGEERROR_H
#define SERIO_STORAGEERROR_H

#include <stdexcept>
#include <string>

namespace serio::qt {
    class StorageError : public std::runtime_error {
    public:
        static StorageError openDatabase(const std::string& databaseName, const std::string& reason);
    private:
        StorageError(const std::string &action, const std::string& reason);
    };
}

#endif //SERIO_STORAGEERROR_H
