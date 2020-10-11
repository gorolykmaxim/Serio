#include <gtest/gtest.h>
#include <QSqlQuery>
#include <QVariant>
#include <database-storage/DatabaseStorage.h>

class DatabaseMigrationTest : public ::testing::Test {
protected:
    serio::qt::DatabaseStorage storage;
    const QList<QVariant> crawlLogEntryRecord = {1, "tv-show", "text", "input", "output"};
    const QList<QVariant> tvShowCrawlerRecord = {"tv-show-name", "crawler body text"};
    const QList<QVariant> tvShowRecord = {"1234", "tv show name", "https://tv-show.com/thumbnail.jpg"};
    const QList<QVariant> episodeRecord = {1, tvShowRecord[0], "Episode 1", "https://tv-show.com/episode-1.mp4"};
    const QList<QVariant> episodeViewRecord = {tvShowRecord[0], episodeRecord[0], 123456, 35.5};
    virtual void SetUp() {
        storage.openDatabase(true);
    }
    void createTable(const QString& query) {
        QSqlQuery(QSqlDatabase::database()).exec(query);
    }
    void populateTable(const QString& query, const std::vector<QList<QVariant>>&& rows) {
        QSqlQuery q(QSqlDatabase::database());
        q.prepare(query);
        QList<QList<QVariant>> values;
        for (const auto& row: rows) {
            for (int i = 0; i < row.size(); i++) {
                if (values.size() < i + 1) {
                    values.push_back(QList<QVariant>());
                }
                values[i].push_back(row[i]);
            }
        }
        for (const auto& column: values) {
            q.addBindValue(column);
        }
        q.execBatch();
    }
    void expectTable(const QString&& name, const std::vector<std::vector<QVariant>>&& columns) {
        QSqlQuery q(QSqlDatabase::database());
        q.exec("PRAGMA TABLE_INFO('" + name + "')");
        for (int i = 0; i < columns.size(); i++) {
            const auto& column = columns[i];
            q.next();
            for (int j = 0; j < 6; j++) {
                EXPECT_EQ(q.value(j), column[j]) << "Expected '" + column[j].toString().toStdString() + "' got '" + q.value(j).toString().toStdString() + "' in column " + std::to_string(i) + " in row " + std::to_string(j);
            }
        }
    }
    void expectTableData(const QString&& name, const QList<QList<QVariant>>&& data) {
        QSqlQuery q(QSqlDatabase::database());
        q.exec("SELECT * FROM " + name);
        for (int i = 0; i < data.size(); i++) {
            q.next();
            const QList<QVariant>& row = data[i];
            for (int j = 0; j < row.size(); j++) {
                EXPECT_EQ(row[j], q.value(j)) << "Expected '" + row[j].toString().toStdString() + "' got '" + q.value(j).toString().toStdString() + "' in column " + std::to_string(i) + " in row " + std::to_string(j);
            }
        }
    }
    void expectTableDoesNotExist(const QString& name) {
        QSqlQuery q(QSqlDatabase::database());
        q.exec("SELECT * FROM " + name);
        EXPECT_FALSE(q.next()) << "Table " + name.toStdString() + " should not exist but it does";
    }
};

TEST_F(DatabaseMigrationTest, shouldCreateCrawlLogEntryTableWithoutMigration) {
    storage.initializeSchema();
    populateTable("INSERT INTO CRAWL_LOG_ENTRY VALUES(?, ?, ?, ?, ?)", {crawlLogEntryRecord});
    storage.initializeSchema();
    expectTable("CRAWL_LOG_ENTRY", {
            {0, "ID", "INTEGER", 1, "", 1},
            {1, "TV_SHOW_NAME", "TEXT", 1, "", 2},
            {2, "TEXT", "TEXT", 1, "", 0},
            {3, "STEP_INPUT_DATA", "TEXT", 0, "", 0},
            {4, "STEP_OUTPUT_DATA", "TEXT", 0, "", 0},
    });
    expectTableData("CRAWL_LOG_ENTRY", {
            {1, "tv-show", "text", "input", "output"}
    });
}

TEST_F(DatabaseMigrationTest, shouldMigrateExistingCrawlLogEntryTable) {
    createTable("CREATE TABLE CRAWL_LOG_ENTRY(ID INTEGER PRIMARY KEY, "
                "SHOW_ID TEXT NOT NULL, VALUE TEXT NOT NULL, INPUT TEXT, OUTPUT TEXT)");
    populateTable("INSERT INTO CRAWL_LOG_ENTRY VALUES(?, ?, ?, ?, ?)", {crawlLogEntryRecord});
    storage.initializeSchema();
    expectTable("CRAWL_LOG_ENTRY", {
            {0, "ID", "INTEGER", 1, "", 1},
            {1, "TV_SHOW_NAME", "TEXT", 1, "", 2},
            {2, "TEXT", "TEXT", 1, "", 0},
            {3, "STEP_INPUT_DATA", "TEXT", 0, "", 0},
            {4, "STEP_OUTPUT_DATA", "TEXT", 0, "", 0},
    });
    expectTableData("CRAWL_LOG_ENTRY", {crawlLogEntryRecord});
}

TEST_F(DatabaseMigrationTest, shouldCreateTvShowCrawlerTableWithoutMigration) {
    storage.initializeSchema();
    populateTable("INSERT INTO TV_SHOW_CRAWLER VALUES(?, ?)", {tvShowCrawlerRecord});
    storage.initializeSchema();
    expectTable("TV_SHOW_CRAWLER", {
            {0, "TV_SHOW_NAME", "TEXT", 1, "", 1},
            {1, "CRAWLER", "TEXT", 1, "", 0}
    });
    expectTableData("TV_SHOW_CRAWLER", {tvShowCrawlerRecord});
}

TEST_F(DatabaseMigrationTest, shouldMigrateExistingTvShowCrawlerTable) {
    createTable("CREATE TABLE SHOW_CRAWLER(SHOW_ID TEXT PRIMARY KEY, CRAWLER TEXT NOT NULL)");
    populateTable("INSERT INTO SHOW_CRAWLER VALUES(?, ?)", {tvShowCrawlerRecord});
    storage.initializeSchema();
    expectTable("TV_SHOW_CRAWLER", {
            {0, "TV_SHOW_NAME", "TEXT", 1, "", 1},
            {1, "CRAWLER", "TEXT", 1, "", 0}
    });
    expectTableData("TV_SHOW_CRAWLER", {tvShowCrawlerRecord});
    expectTableDoesNotExist("SHOW_CRAWLER");
}

TEST_F(DatabaseMigrationTest, shouldCreateTvShowTableGroupWithoutMigration) {
    storage.initializeSchema();
    populateTable("INSERT INTO TV_SHOW VALUES(?, ?)", {{tvShowRecord[1], tvShowRecord[2]}});
    populateTable("INSERT INTO EPISODE VALUES(?, ?, ?, ?)", {{episodeRecord[0], tvShowRecord[1], episodeRecord[2], episodeRecord[3]}});
    populateTable("INSERT INTO EPISODE_VIEW VALUES(?, ?, ?, ?)", {{tvShowRecord[1], episodeViewRecord[1], episodeViewRecord[2], episodeViewRecord[3]}});
    storage.initializeSchema();
    expectTable("TV_SHOW", {
            {0, "NAME", "TEXT", 1, "", 1},
            {1, "THUMBNAIL_URL", "TEXT", 1, "", 0}
    });
    expectTable("EPISODE", {
            {0, "ID", "UNSIGNED BIG INT", 1, "", 1},
            {1, "TV_SHOW_NAME", "TEXT", 1, "", 2},
            {2, "NAME", "TEXT", 1, "", 0},
            {3, "VIDEO_URL", "TEXT", 1, "", 0}
    });
    expectTable("EPISODE_VIEW", {
            {0, "TV_SHOW_NAME", "TEXT", 1, "", 1},
            {1, "EPISODE_ID", "UNSIGNED BIG INT", 1, "", 2},
            {2, "LAST_WATCH_DATE", "BIGINT", 1, "", 0},
            {3, "WATCH_PROGRESS", "REAL", 1, "", 0},
    });
    expectTableData("TV_SHOW", {{tvShowRecord[1], tvShowRecord[2]}});
    expectTableData("EPISODE", {{episodeRecord[0], tvShowRecord[1], episodeRecord[2], episodeRecord[3]}});
    expectTableData("EPISODE_VIEW", {{tvShowRecord[1], episodeViewRecord[1], episodeViewRecord[2], episodeViewRecord[3]}});
}

TEST_F(DatabaseMigrationTest, shouldMigrateExistingTvShowTableGroup) {
    createTable("CREATE TABLE \"SHOW\"(ID TEXT PRIMARY KEY, NAME TEXT NOT NULL, THUMBNAIL_URL TEXT NOT NULL)");
    createTable("CREATE TABLE EPISODE(ID INTEGER NOT NULL, SHOW_ID TEXT NOT NULL, NAME TEXT NOT NULL, VIDEO_URL TEXT NOT NULL, PRIMARY KEY(ID, SHOW_ID), CONSTRAINT FK_SHOW FOREIGN KEY (SHOW_ID) REFERENCES \"SHOW\"(ID) ON DELETE CASCADE)");
    createTable("CREATE TABLE EPISODE_VIEW(SHOW_ID TEXT NOT NULL, EPISODE_ID TEXT NOT NULL, LAST_WATCH_DATE BIGINT NOT NULL, PROGRESS REAL NOT NULL, PRIMARY KEY(SHOW_ID, EPISODE_ID))");
    populateTable("INSERT INTO SHOW VALUES(?, ?, ?)", {tvShowRecord});
    populateTable("INSERT INTO EPISODE VALUES(?, ?, ?, ?)", {episodeRecord});
    populateTable("INSERT INTO EPISODE_VIEW VALUES(?, ?, ?, ?)", {episodeViewRecord});
    storage.initializeSchema();
    expectTable("TV_SHOW", {
            {0, "NAME", "TEXT", 1, "", 1},
            {1, "THUMBNAIL_URL", "TEXT", 1, "", 0}
    });
    expectTableData("TV_SHOW", {{tvShowRecord[1], tvShowRecord[2]}});
    expectTable("EPISODE", {
            {0, "ID", "UNSIGNED BIG INT", 1, "", 1},
            {1, "TV_SHOW_NAME", "TEXT", 1, "", 2},
            {2, "NAME", "TEXT", 1, "", 0},
            {3, "VIDEO_URL", "TEXT", 1, "", 0}
    });
    expectTableData("EPISODE", {{episodeRecord[0], tvShowRecord[1], episodeRecord[2], episodeRecord[3]}});
    expectTable("EPISODE_VIEW", {
            {0, "TV_SHOW_NAME", "TEXT", 1, "", 1},
            {1, "EPISODE_ID", "UNSIGNED BIG INT", 1, "", 2},
            {2, "LAST_WATCH_DATE", "BIGINT", 1, "", 0},
            {3, "WATCH_PROGRESS", "REAL", 1, "", 0},
    });
    expectTableData("EPISODE_VIEW", {{tvShowRecord[1], episodeViewRecord[1], episodeViewRecord[2], episodeViewRecord[3]}});
    expectTableDoesNotExist("SHOW");
}
