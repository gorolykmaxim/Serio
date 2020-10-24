#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include "DatabaseTvShowCrawlerStorage.h"

namespace serio::qt {

std::optional<std::string> DatabaseTvShowCrawlerStorage::getTvShowCrawlerByTvShowName(const std::string &tvShowName) const {
    auto findTvShowCrawler = createAndExec("SELECT CRAWLER FROM TV_SHOW_CRAWLER WHERE TV_SHOW_NAME = ?",
                                           QString::fromStdString(tvShowName));
    if (findTvShowCrawler.next()) {
        return findTvShowCrawler.value(0).toString().toStdString();
    } else {
        return {};
    }
}

void DatabaseTvShowCrawlerStorage::deleteTvShowCrawlerOfTvShow(const std::string &tvShowName) const {
    createAndExec("DELETE FROM TV_SHOW_CRAWLER WHERE TV_SHOW_NAME = ?", QString::fromStdString(tvShowName));
}

void DatabaseTvShowCrawlerStorage::insertTvShowCrawler(const std::string &tvShowName, const std::string &serializedCrawler) const {
    QSqlQuery insertTvShowCrawler(QSqlDatabase::database());
    insertTvShowCrawler.prepare("INSERT INTO TV_SHOW_CRAWLER VALUES(?, ?)");
    insertTvShowCrawler.addBindValue(QString::fromStdString(tvShowName));
    insertTvShowCrawler.addBindValue(QString::fromStdString(serializedCrawler));
    insertTvShowCrawler.exec();
}

void DatabaseTvShowCrawlerStorage::createNewVersion() const {
    createAndExec("CREATE TABLE IF NOT EXISTS TV_SHOW_CRAWLER("
                  "TV_SHOW_NAME TEXT NOT NULL PRIMARY KEY, "
                  "CRAWLER TEXT NOT NULL)");
}

void DatabaseTvShowCrawlerStorage::migrateRecordsFromOldVersion() const {
    createAndExec("INSERT INTO TV_SHOW_CRAWLER "
                  "SELECT S.NAME, C.CRAWLER "
                  "FROM SHOW_CRAWLER C "
                  "LEFT JOIN SHOW S ON C.SHOW_ID = S.ID");
}

void DatabaseTvShowCrawlerStorage::dropOldVersion() const {
    createAndExec("DROP TABLE SHOW_CRAWLER");
}

}