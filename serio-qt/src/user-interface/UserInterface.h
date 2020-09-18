#ifndef SERIO_USERINTERFACE_H
#define SERIO_USERINTERFACE_H

#include <QQmlApplicationEngine>
#include <user-interface/view-model/AllTvShowsViewModel.h>
#include <database-storage/DatabaseStorage.h>
#include <user-interface/view-model/TvShowCrawlerEditorViewModel.h>
#include <user-interface/view-model/CrawlerEditorViewModel.h>
#include <user-interface/view-model/CrawlerStepEditorViewModel.h>
#include <http-client/QHttpClient.h>

namespace serio::qt {

class UserInterface : public QObject {
    Q_OBJECT
public:
    UserInterface(QHttpClient& httpClient, core::TvShowCrawlerEditor& tvShowCrawlerEditor, DatabaseStorage& storage,
                  QTaskExecutor& executor);
    void initialize();
private:
    QQmlApplicationEngine engine;
    ActionRouter router;
    StackOfViews stack;
    AllTvShowsViewModel allTvShowsViewModel;
    TvShowCrawlerEditorViewModel tvShowCrawlerEditorViewModel;
    CrawlerEditorViewModel crawlerEditorViewModel;
    CrawlerStepEditorViewModel crawlerStepEditorViewModel;
    void loadFonts();
    void exitOnUiLoadFailure(const QUrl &url);
};

}

#endif //SERIO_USERINTERFACE_H
