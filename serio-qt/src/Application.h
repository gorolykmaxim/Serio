#ifndef SERIO_APPLICATION_H
#define SERIO_APPLICATION_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <storage/TvShowStorage.h>
#include <view-model/AllTvShowsViewModel.h>
#include <concurrency/TaskExecutor.h>

namespace serio::qt {
    class Application : public QGuiApplication {
    public:
        Application(int &argc, char **argv);
    private:
        TaskExecutor executor;
        TvShowStorage storage;
        QQmlApplicationEngine engine;
        AllTvShowsViewModel allTvShowsViewModel;
        void loadFonts();
        void initializeDatabase();
        std::string getPathToDatabaseFile() const;
        void initializeStorageInDatabase(const std::string& databaseFilePath);
        void initializeQmlEngine();
        void exitOnUiLoadFailure(const QUrl &url);
        void registerViewModelsInEngine();
    };
}

#endif //SERIO_APPLICATION_H
