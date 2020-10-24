#include "BaseDatabaseStorage.h"

namespace serio::qt {

QSqlQuery BaseDatabaseStorage::createAndExec(const QString &sql, const QVariant &argument) const {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(sql);
    query.addBindValue(argument);
    query.exec();
    return query;
}

QSqlQuery BaseDatabaseStorage::createAndExec(const QString &sql) const {
    QSqlQuery query(QSqlDatabase::database());
    query.exec(sql);
    return query;
}

}