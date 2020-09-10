#include "Application.h"

serio::qt::Application::Application(int &argc, char **argv)
    : QGuiApplication(argc, argv), userInterface(storage, executor) {
    initializeStorageInDatabase();
    userInterface.initialize();
}

void serio::qt::Application::initializeStorageInDatabase() {
    QFutureWatcher<void> dbInitFuture;
    executor.runInBackground(dbInitFuture, &storage, &DatabaseStorage::initialize, false);
    dbInitFuture.waitForFinished();
}
