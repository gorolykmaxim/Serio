#include <QDir>
#include <QFontDatabase>
#include <QQmlContext>
#include <storage/DatabaseStorage.h>
#include "Application.h"

serio::qt::Application::Application(int &argc, char **argv)
    : QGuiApplication(argc, argv), userInterface(storage, executor) {
    initializeStorageInDatabase(getPathToDatabaseFile());
    userInterface.initialize();
}

std::string serio::qt::Application::getPathToDatabaseFile() const {
    QString home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return QDir(home).filePath(".serio.dev.db").toStdString();
}

void serio::qt::Application::initializeStorageInDatabase(const std::string &databaseFilePath) {
    QFutureWatcher<void> dbInitFuture;
    executor.runInBackground(dbInitFuture, &storage, &DatabaseStorage::initialize, databaseFilePath);
    dbInitFuture.waitForFinished();
}
