#include <QSqlQuery>
#include <QSqlDatabase>
#include <QString>
#include <QVariantList>
#include "DatabaseTvShowCrawlerLogStorage.h"

namespace serio::qt {

void DatabaseTvShowCrawlerLogStorage::saveCrawlLog(const std::string &tvShowName,
                                                   const std::vector<core::CrawlLogEntry> &log) const {
    createAndExec("DELETE FROM CRAWL_LOG_ENTRY WHERE TV_SHOW_NAME = ?", QString::fromStdString(tvShowName));
    insertCrawlLogEntries(tvShowName, log);
}

std::vector<core::CrawlLogEntry> DatabaseTvShowCrawlerLogStorage::getLastCrawlLogOfTvShow(const std::string &tvShowName) const {
    std::vector<core::CrawlLogEntry> result;
    auto findLastCrawlLogOfTvShow = createAndExec(
            "SELECT TEXT, STEP_INPUT_DATA, STEP_OUTPUT_DATA FROM CRAWL_LOG_ENTRY WHERE TV_SHOW_NAME = ? ORDER BY ID",
            QString::fromStdString(tvShowName));
    while (findLastCrawlLogOfTvShow.next()) {
        result.push_back(readCrawlLogEntryFrom(findLastCrawlLogOfTvShow));
    }
    return result;
}

void DatabaseTvShowCrawlerLogStorage::insertCrawlLogEntries(const std::string &tvShowName,
                                                            const std::vector<core::CrawlLogEntry> &entries) const {
    QSqlQuery insertCrawlLog(QSqlDatabase::database());
    insertCrawlLog.prepare("INSERT INTO CRAWL_LOG_ENTRY VALUES(?, ?, ?, ?, ?)");
    auto name = QString::fromStdString(tvShowName);
    QVariantList ids, tvShowNames, texts, stepInputData, stepOutputData;
    for (int i = 0; i < entries.size(); i++) {
        const auto& entry = entries[i];
        ids << i;
        tvShowNames << name;
        texts << QString::fromStdString(entry.getText());
        stepInputData << QString::fromStdString(entry.getStepInputData());
        stepOutputData << QString::fromStdString(entry.getStepOutputData());
    }
    insertCrawlLog.addBindValue(ids);
    insertCrawlLog.addBindValue(tvShowNames);
    insertCrawlLog.addBindValue(texts);
    insertCrawlLog.addBindValue(stepInputData);
    insertCrawlLog.addBindValue(stepOutputData);
    insertCrawlLog.execBatch();
}

core::CrawlLogEntry DatabaseTvShowCrawlerLogStorage::readCrawlLogEntryFrom(const QSqlQuery &query) const {
    auto text = query.value(0).toString().toStdString();
    auto stepInputData = query.value(1).toString().toStdString();
    auto stepOutputData = query.value(2).toString().toStdString();
    return core::CrawlLogEntry(text, stepInputData, stepOutputData);
}

void DatabaseTvShowCrawlerLogStorage::backupOldVersion() const {
    createAndExec("ALTER TABLE CRAWL_LOG_ENTRY RENAME TO OLD_CRAWL_LOG_ENTRY");
}

void DatabaseTvShowCrawlerLogStorage::createNewVersion() const {
    createAndExec("CREATE TABLE IF NOT EXISTS CRAWL_LOG_ENTRY("
                  "ID INTEGER NOT NULL, "
                  "TV_SHOW_NAME TEXT NOT NULL, "
                  "\"TEXT\" TEXT NOT NULL, "
                  "STEP_INPUT_DATA TEXT, "
                  "STEP_OUTPUT_DATA TEXT, "
                  "PRIMARY KEY (ID, TV_SHOW_NAME))");
}

void DatabaseTvShowCrawlerLogStorage::migrateRecordsFromOldVersion() const {
    createAndExec("INSERT INTO CRAWL_LOG_ENTRY "
                  "SELECT E.ID, S.NAME, E.VALUE, E.INPUT, E.OUTPUT "
                  "FROM OLD_CRAWL_LOG_ENTRY E "
                  "INNER JOIN SHOW S ON E.SHOW_ID = S.ID");
}

void DatabaseTvShowCrawlerLogStorage::dropOldVersion() const {
    createAndExec("DROP TABLE OLD_CRAWL_LOG_ENTRY");
}

}