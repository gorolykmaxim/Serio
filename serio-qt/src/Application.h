#ifndef SERIO_APPLICATION_H
#define SERIO_APPLICATION_H

#include <QGuiApplication>
#include <database-storage/DatabaseStorage.h>
#include <task-executor/QTaskExecutor.h>
#include <user-interface/UserInterface.h>
#include <http-client/QHttpClient.h>

namespace serio::qt {

class Application : public QGuiApplication {
public:
    Application(int &argc, char **argv);
private:
    QHttpClient httpClient;
    core::TvShowCrawlerRuntime tvShowCrawlerRuntime;
    core::TvShowCrawlerEditor tvShowCrawlerEditor;
    QTaskExecutor executor;
    DatabaseStorage storage;
    UserInterface userInterface;
};

}

#endif //SERIO_APPLICATION_H
