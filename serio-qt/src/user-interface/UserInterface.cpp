#include <QString>
#include <QDir>
#include <QFontDatabase>
#include <QCoreApplication>
#include <QQmlContext>
#include "UserInterface.h"

serio::qt::UserInterface::UserInterface(serio::qt::QHttpClient& httpClient,
                                        serio::core::TvShowCrawlerEditor &tvShowCrawlerEditor,
                                        serio::qt::DatabaseStorage &storage, serio::qt::QTaskExecutor &executor)
        : QObject(),
          router(executor, engine),
          errorViewModel(stack),
          allTvShowsViewModel(100, 2, storage),
          tvShowCrawlerEditorViewModel(tvShowCrawlerEditor, stack),
          crawlerEditorViewModel(tvShowCrawlerEditor, stack),
          crawlerStepEditorViewModel(tvShowCrawlerEditor, stack) {
    httpClient.assignTo(engine);
    stack.initialize(router, engine);
    errorViewModel.initialize(router, engine);
    allTvShowsViewModel.initialize(router, engine);
    tvShowCrawlerEditorViewModel.initialize(router, engine);
    crawlerEditorViewModel.initialize(router, engine);
    crawlerStepEditorViewModel.initialize(router, engine);
    loadFonts();
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