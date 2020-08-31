#include <QString>
#include <QDir>
#include <QFontDatabase>
#include <QCoreApplication>
#include <QQmlContext>
#include <user-interface/model/TvShowListModel.h>
#include "UserInterface.h"

serio::qt::UserInterface::UserInterface(serio::qt::DatabaseStorage &storage, serio::qt::QTaskExecutor &executor)
        : QObject(), allTvShowsViewModel(100, 2, storage, executor) {
    loadFonts();
    registerViewModelsInEngine();
    registerQmlTypes();
}

void serio::qt::UserInterface::loadFonts() {
    QString fontsDir = ":/fonts/";
    for (const QString& font: QDir(fontsDir).entryList()) {
        QFontDatabase::addApplicationFont(fontsDir + font);
    }
}

void serio::qt::UserInterface::initialize() {
    const QUrl rootUrl(QStringLiteral("qrc:/qml/main.qml"));
    exitOnUiLoadFailure(rootUrl);
    engine.load(rootUrl);
}

void serio::qt::UserInterface::exitOnUiLoadFailure(const QUrl &url) {
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, this, [url] (QObject* obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            QCoreApplication::exit(1);
        }
    }, Qt::QueuedConnection);
}

void serio::qt::UserInterface::registerViewModelsInEngine() {
    engine.rootContext()->setContextProperty("allTvShowsViewModel", &allTvShowsViewModel);
}

void serio::qt::UserInterface::registerQmlTypes() {
    qmlRegisterUncreatableType<TvShowListModel>("Serio", 1, 0, "TvShowListModel", nullptr);
}
