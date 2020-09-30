#include <QString>
#include <QDir>
#include <QFontDatabase>
#include <QCoreApplication>
#include <QQmlContext>
#include "UserInterface.h"

serio::qt::UserInterface::UserInterface(serio::qt::QHttpClient& httpClient,
                                        serio::core::TvShowCrawlerEditor &tvShowCrawlerEditor,
                                        serio::qt::DatabaseStorage &storage, serio::core::TvShowViewer& viewer,
                                        serio::qt::QTaskExecutor &executor)
        : QObject(),
          router(executor),
          dialog(stack),
          errorViewModel(dialog),
          allTvShowsViewModel(100, 2, storage, viewer, stack),
          crawlLogViewModel(tvShowCrawlerEditor, storage, stack),
          tvShowCrawlerEditorViewModel(tvShowCrawlerEditor, viewer, dialog, stack),
          crawlerEditorViewModel(tvShowCrawlerEditor, stack),
          crawlerStepEditorViewModel(tvShowCrawlerEditor, stack),
          tvShowViewModel(100, 2, viewer, dialog, snackbar, stack) {
    httpClient.assignTo(engine);
    loadFonts();
}

void serio::qt::UserInterface::loadFonts() {
    QString fontsDir = ":/fonts/";
    for (const QString& font: QDir(fontsDir).entryList()) {
        QFontDatabase::addApplicationFont(fontsDir + font);
    }
}

void serio::qt::UserInterface::initialize() {
    stack.initialize(router, engine);
    dialog.initialize(engine);
    snackbar.initialize(engine);
    router.initialize(engine);
    errorViewModel.initialize(router);
    allTvShowsViewModel.initialize(router, engine);
    crawlLogViewModel.initialize(router, engine);
    tvShowCrawlerEditorViewModel.initialize(router, engine);
    crawlerEditorViewModel.initialize(router, engine);
    crawlerStepEditorViewModel.initialize(router, engine);
    tvShowViewModel.initialize(router, engine);
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