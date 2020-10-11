#include "Schema.h"

void serio::qt::Schema::initialize(bool migrate) const {
    if (migrate) {
        backupOldVersion();
    }
    createNewVersion();
    if (migrate) {
        migrateRecordsFromOldVersion();
        dropOldVersion();
    }
}

void serio::qt::Schema::backupOldVersion() const {}

void serio::qt::Schema::createNewVersion() const {}

void serio::qt::Schema::migrateRecordsFromOldVersion() const {}

void serio::qt::Schema::dropOldVersion() const {}
