#include <QString>
#include <QDir>
#include <QFontDatabase>
#include <QCoreApplication>
#include <QQmlContext>
#include "UserInterface.h"
#include "ViewNames.h"

namespace serio::qt {

UserInterface::UserInterface(QHttpClient& httpClient, core::TvShowCrawlerEditor &tvShowCrawlerEditor,
                             DatabaseStorage &storage, core::TvShowViewer& viewer, core::TvShowPlayer& tvShowPlayer,
                             QTaskExecutor &executor)
    : QObject(),
      router(executor),
      background({tvShowView, tvShowDetailsView}),
      dialog(stack),
      errorViewModel(dialog),
      allTvShowsViewModel(100, 2, storage, viewer, stack),
      crawlLogViewModel(tvShowCrawlerEditor, storage, stack),
      tvShowCrawlerEditorViewModel(tvShowCrawlerEditor, viewer, dialog, stack),
      crawlerEditorViewModel(tvShowCrawlerEditor, stack),
      crawlerStepEditorViewModel(tvShowCrawlerEditor, stack),
      tvShowViewModel(100, 2, viewer, dialog, background, snackbar, stack),
      tvShowPlayerViewModel(tvShowPlayer, dialog, stack) {
    useWmfVideoPlayerBackend();
    httpClient.assignTo(engine);
    loadFonts();
}

void UserInterface::useWmfVideoPlayerBackend() const {
    qputenv("QT_MULTIMEDIA_PREFERRED_PLUGINS", "windowsmediafoundation");
}

void UserInterface::loadFonts() const {
    auto fontsDir = ":/fonts/";
    for (const auto& font: QDir(fontsDir).entryList()) {
        QFontDatabase::addApplicationFont(fontsDir + font);
    }
}

void UserInterface::initialize() {
    stack.initialize(router, engine);
    background.initialize(stack, engine);
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
    tvShowPlayerViewModel.initialize(router, engine);
    const QUrl rootUrl(QStringLiteral("qrc:/qml/main.qml"));
    exitOnUiLoadFailure(rootUrl);
    engine.load(rootUrl);
}

void UserInterface::exitOnUiLoadFailure(const QUrl &url) const {
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, this, [url] (auto obj, const auto& objUrl) {
        if (!obj && url == objUrl) {
            QCoreApplication::exit(1);
        }
    }, Qt::QueuedConnection);
}

}
