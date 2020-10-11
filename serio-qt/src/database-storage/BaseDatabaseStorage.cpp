#include "BaseDatabaseStorage.h"

QSqlQuery serio::qt::BaseDatabaseStorage::createAndExec(const QString &sql, const QVariant &argument) const {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(sql);
    query.addBindValue(argument);
    query.exec();
    return query;
}

QSqlQuery serio::qt::BaseDatabaseStorage::createAndExec(const QString &sql) const {
    QSqlQuery query(QSqlDatabase::database());
    query.exec(sql);
    return query;
}
