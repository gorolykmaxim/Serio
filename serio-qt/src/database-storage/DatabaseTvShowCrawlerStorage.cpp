#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include "DatabaseTvShowCrawlerStorage.h"

void serio::qt::DatabaseTvShowCrawlerStorage::initialize() const {
    QSqlQuery createTvShowCrawlers(QSqlDatabase::database());
    createTvShowCrawlers.exec("CREATE TABLE IF NOT EXISTS TV_SHOW_CRAWLER(TV_SHOW_NAME TEXT PRIMARY KEY, CRAWLER TEXT NOT NULL)");
}

std::optional<std::string> serio::qt::DatabaseTvShowCrawlerStorage::getTvShowCrawlerByTvShowName(const std::string &tvShowName) const {
    auto findTvShowCrawler = createAndExec("SELECT CRAWLER FROM TV_SHOW_CRAWLER WHERE TV_SHOW_NAME = ?",
                                                QString::fromStdString(tvShowName));
    if (findTvShowCrawler.next()) {
        return findTvShowCrawler.value(0).toString().toStdString();
    } else {
        return {};
    }
}

void serio::qt::DatabaseTvShowCrawlerStorage::deleteTvShowCrawlerOfTvShow(const std::string &tvShowName) const {
    createAndExec("DELETE FROM TV_SHOW_CRAWLER WHERE TV_SHOW_NAME = ?", QString::fromStdString(tvShowName));
}

void serio::qt::DatabaseTvShowCrawlerStorage::insertTvShowCrawler(const std::string &tvShowName, const std::string &serializedCrawler) const {
    QSqlQuery insertTvShowCrawler(QSqlDatabase::database());
    insertTvShowCrawler.prepare("INSERT INTO TV_SHOW_CRAWLER VALUES(?, ?)");
    insertTvShowCrawler.addBindValue(QString::fromStdString(tvShowName));
    insertTvShowCrawler.addBindValue(QString::fromStdString(serializedCrawler));
    insertTvShowCrawler.exec();
}
