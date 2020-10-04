#include <QSqlQuery>
#include <QSqlDatabase>
#include <QString>
#include <QVariantList>
#include "DatabaseTvShowCrawlerLogStorage.h"

void serio::qt::DatabaseTvShowCrawlerLogStorage::initialize() const {
    QSqlQuery createCrawlLogEntry(QSqlDatabase::database());
    createCrawlLogEntry.exec("CREATE TABLE IF NOT EXISTS CRAWL_LOG_ENTRY("
                                "ID INTEGER NOT NULL, "
                                "TV_SHOW_NAME TEXT NOT NULL, "
                                "\"TEXT\" TEXT NOT NULL, "
                                "STEP_INPUT_DATA TEXT, "
                                "STEP_OUTPUT_DATA TEXT, "
                                "PRIMARY KEY (ID, TV_SHOW_NAME))");
}

void serio::qt::DatabaseTvShowCrawlerLogStorage::saveCrawlLog(const std::string &tvShowName, const std::vector<core::CrawlLogEntry> &log) const {
    createAndExec("DELETE FROM CRAWL_LOG_ENTRY WHERE TV_SHOW_NAME = ?", QString::fromStdString(tvShowName));
    insertCrawlLogEntries(tvShowName, log);
}

std::vector<serio::core::CrawlLogEntry> serio::qt::DatabaseTvShowCrawlerLogStorage::getLastCrawlLogOfTvShow(const std::string &tvShowName) const {
    std::vector<serio::core::CrawlLogEntry> result;
    QSqlQuery findLastCrawlLogOfTvShow = createAndExec(
            "SELECT TEXT, STEP_INPUT_DATA, STEP_OUTPUT_DATA FROM CRAWL_LOG_ENTRY WHERE TV_SHOW_NAME = ? ORDER BY ID",
            QString::fromStdString(tvShowName));
    while (findLastCrawlLogOfTvShow.next()) {
        result.push_back(readCrawlLogEntryFrom(findLastCrawlLogOfTvShow));
    }
    return result;
}

void serio::qt::DatabaseTvShowCrawlerLogStorage::insertCrawlLogEntries(const std::string &tvShowName,
                                                                       const std::vector<core::CrawlLogEntry> &entries) const {
    QSqlQuery insertCrawlLog(QSqlDatabase::database());
    insertCrawlLog.prepare("INSERT INTO CRAWL_LOG_ENTRY VALUES(?, ?, ?, ?, ?)");
    QString name = QString::fromStdString(tvShowName);
    QVariantList ids, tvShowNames, texts, stepInputData, stepOutputData;
    for (int i = 0; i < entries.size(); i++) {
        const serio::core::CrawlLogEntry& entry = entries[i];
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

serio::core::CrawlLogEntry serio::qt::DatabaseTvShowCrawlerLogStorage::readCrawlLogEntryFrom(const QSqlQuery &query) const {
    std::string text = query.value(0).toString().toStdString();
    std::string stepInputData = query.value(1).toString().toStdString();
    std::string stepOutputData = query.value(2).toString().toStdString();
    return serio::core::CrawlLogEntry(text, stepInputData, stepOutputData);
}
