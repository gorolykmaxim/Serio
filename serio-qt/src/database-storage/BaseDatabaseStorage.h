#ifndef SERIO_BASEDATABASESTORAGE_H
#define SERIO_BASEDATABASESTORAGE_H

#include <QSqlQuery>

namespace serio::qt {

class BaseDatabaseStorage {
protected:
    QSqlQuery createAndExec(const QString& sql, const QVariant& argument) const;
};

}

#endif //SERIO_BASEDATABASESTORAGE_H
