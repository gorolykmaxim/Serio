#include "StorageError.h"
#include <sstream>

serio::qt::StorageError serio::qt::StorageError::openDatabase(const std::string& databaseName, const std::string& reason) {
    std::stringstream action = std::stringstream("open database '") << databaseName << "'";
    return serio::qt::StorageError(action.str(), reason);
}

serio::qt::StorageError::StorageError(const std::string& action, const std::string& reason)
    : runtime_error((std::stringstream("Failed to ") << action << ". Reason: " << reason).str()) {}
