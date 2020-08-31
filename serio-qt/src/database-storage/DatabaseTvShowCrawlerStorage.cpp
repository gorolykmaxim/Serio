#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include "DatabaseTvShowCrawlerStorage.h"

void serio::qt::DatabaseTvShowCrawlerStorage::initialize() {
    QSqlQuery createTvShowCrawlers(QSqlDatabase::database());
    createTvShowCrawlers.exec("CREATE TABLE IF NOT EXISTS TV_SHOW_CRAWLER(TV_SHOW_NAME TEXT PRIMARY KEY, CRAWLER TEXT NOT NULL)");
}

std::optional<std::string> serio::qt::DatabaseTvShowCrawlerStorage::getTvShowCrawlerByTvShowName(const std::string &tvShowName) {
    QSqlQuery findTvShowCrawler(QSqlDatabase::database());
    findTvShowCrawler.prepare("SELECT CRAWLER FROM TV_SHOW_CRAWLER WHERE TV_SHOW_NAME = ?");
    findTvShowCrawler.addBindValue(QString::fromStdString(tvShowName));
    findTvShowCrawler.exec();
    if (findTvShowCrawler.next()) {
        return findTvShowCrawler.value(0).toString().toStdString();
    } else {
        return {};
    }
}

void serio::qt::DatabaseTvShowCrawlerStorage::deleteTvShowCrawlerOfTvShow(const std::string &tvShowName) {
    QSqlQuery deleteTvShowCrawler(QSqlDatabase::database());
    deleteTvShowCrawler.prepare("DELETE FROM TV_SHOW_CRAWLER WHERE TV_SHOW_NAME = ?");
    deleteTvShowCrawler.addBindValue(QString::fromStdString(tvShowName));
    deleteTvShowCrawler.exec();
}

void serio::qt::DatabaseTvShowCrawlerStorage::insertTvShowCrawler(const std::string &tvShowName, const std::string &serializedCrawler) {
    QSqlQuery insertTvShowCrawler(QSqlDatabase::database());
    insertTvShowCrawler.prepare("INSERT INTO TV_SHOW_CRAWLER VALUES(?, ?)");
    insertTvShowCrawler.addBindValue(QString::fromStdString(tvShowName));
    insertTvShowCrawler.addBindValue(QString::fromStdString(serializedCrawler));
    insertTvShowCrawler.exec();
}
