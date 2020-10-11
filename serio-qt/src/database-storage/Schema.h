#ifndef SERIO_SCHEMA_H
#define SERIO_SCHEMA_H

namespace serio::qt {

class Schema {
public:
    void initialize(bool migrate) const;
protected:
    virtual void backupOldVersion() const;
    virtual void createNewVersion() const;
    virtual void migrateRecordsFromOldVersion() const;
    virtual void dropOldVersion() const;
};

}

#endif //SERIO_SCHEMA_H
