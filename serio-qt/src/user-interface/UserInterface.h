#ifndef SERIO_USERINTERFACE_H
#define SERIO_USERINTERFACE_H

#include <QQmlApplicationEngine>
#include <user-interface/view-model/AllTvShowsViewModel.h>
#include <database-storage/DatabaseStorage.h>
#include <user-interface/view-model/TvShowCrawlerEditorViewModel.h>
#include <user-interface/view-model/CrawlerEditorViewModel.h>
#include <user-interface/view-model/CrawlerStepEditorViewModel.h>
#include <http-client/QHttpClient.h>
#include <user-interface/view-model/ErrorViewModel.h>
#include <user-interface/view-model/CrawlLogViewModel.h>
#include <user-interface/view-model/TvShowViewModel.h>
#include <user-interface/view-model/SnackbarViewModel.h>
#include <user-interface/view-model/DialogViewModel.h>
#include <user-interface/view-model/BackgroundViewModel.h>
#include <tv-show-player/TvShowPlayer.h>
#include <user-interface/view-model/TvShowPlayerViewModel.h>

namespace serio::qt {

class UserInterface : public QObject {
    Q_OBJECT
public:
    UserInterface(QHttpClient& httpClient, core::TvShowCrawlerEditor& tvShowCrawlerEditor, DatabaseStorage& storage,
                  core::TvShowViewer& viewer, core::TvShowPlayer& tvShowPlayer, QTaskExecutor& executor);
    void initialize();
private:
    ActionRouter router;
    StackOfViews stack;
    BackgroundViewModel background;
    DialogViewModel dialog;
    SnackbarViewModel snackbar;
    ErrorViewModel errorViewModel;
    AllTvShowsViewModel allTvShowsViewModel;
    CrawlLogViewModel crawlLogViewModel;
    TvShowCrawlerEditorViewModel tvShowCrawlerEditorViewModel;
    CrawlerEditorViewModel crawlerEditorViewModel;
    CrawlerStepEditorViewModel crawlerStepEditorViewModel;
    TvShowViewModel tvShowViewModel;
    TvShowPlayerViewModel tvShowPlayerViewModel;
    QQmlApplicationEngine engine;
    void loadFonts() const;
    void exitOnUiLoadFailure(const QUrl &url) const;
};

}

#endif //SERIO_USERINTERFACE_H
