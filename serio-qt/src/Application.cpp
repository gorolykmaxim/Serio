#include <QDir>
#include <QFontDatabase>
#include <QQmlContext>
#include <storage/DatabaseStorage.h>
#include "Application.h"

serio::qt::Application::Application(int &argc, char **argv)
    : QGuiApplication(argc, argv), allTvShowsViewModel(100, 2, storage, executor) {
    loadFonts();
    initializeDatabase();
    initializeQmlEngine();
}

void serio::qt::Application::loadFonts() {
    QString fontsDir = ":/fonts/";
    for (const QString& font: QDir(fontsDir).entryList()) {
        QFontDatabase::addApplicationFont(fontsDir + font);
    }
}

void serio::qt::Application::initializeQmlEngine() {
    const QUrl rootUrl(QStringLiteral("qrc:/qml/main.qml"));
    exitOnUiLoadFailure(rootUrl);
    registerViewModelsInEngine();
    registerQmlTypes();
    engine.load(rootUrl);
}

void serio::qt::Application::exitOnUiLoadFailure(const QUrl &url) {
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, this, [url] (QObject* obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            QCoreApplication::exit(1);
        }
    }, Qt::QueuedConnection);
}

void serio::qt::Application::registerViewModelsInEngine() {
    engine.rootContext()->setContextProperty("allTvShowsViewModel", &allTvShowsViewModel);
}

void serio::qt::Application::registerQmlTypes() {
    qmlRegisterUncreatableType<TvShowListModel>("Serio", 1, 0, "TvShowListModel", nullptr);
}

void serio::qt::Application::initializeDatabase() {
    initializeStorageInDatabase(getPathToDatabaseFile());
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
