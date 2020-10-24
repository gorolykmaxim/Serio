#include "Schema.h"

namespace serio::qt {

void Schema::initialize(bool migrate) const {
    if (migrate) {
        backupOldVersion();
    }
    createNewVersion();
    if (migrate) {
        migrateRecordsFromOldVersion();
        dropOldVersion();
    }
}

void Schema::backupOldVersion() const {}

void Schema::createNewVersion() const {}

void Schema::migrateRecordsFromOldVersion() const {}

void Schema::dropOldVersion() const {}

}