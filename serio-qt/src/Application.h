#ifndef SERIO_APPLICATION_H
#define SERIO_APPLICATION_H

#include <QGuiApplication>
#include <database-storage/DatabaseStorage.h>
#include <task-executor/QTaskExecutor.h>
#include <user-interface/UserInterface.h>
#include <http-client/QHttpClient.h>
#include <tv-show-player/TvShowPlayer.h>

namespace serio::qt {

class Application : public QGuiApplication {
public:
    Application(int &argc, char **argv);
private:
    QHttpClient httpClient;
    core::TvShowCrawlerRuntime tvShowCrawlerRuntime;
    core::TvShowCrawlerEditor tvShowCrawlerEditor;
    core::TvShowPlayer tvShowPlayer;
    QTaskExecutor executor;
    DatabaseStorage storage;
    core::TvShowViewer viewer;
    UserInterface userInterface;
};

}

#endif //SERIO_APPLICATION_H
