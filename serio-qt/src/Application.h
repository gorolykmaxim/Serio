#ifndef SERIO_APPLICATION_H
#define SERIO_APPLICATION_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <storage/DatabaseTvShowStorage.h>
#include <view-model/AllTvShowsViewModel.h>
#include <concurrency/TaskExecutor.h>

namespace serio::qt {

class Application : public QGuiApplication {
public:
    Application(int &argc, char **argv);
private:
    TaskExecutor executor;
    DatabaseTvShowStorage storage;
    QQmlApplicationEngine engine;
    AllTvShowsViewModel allTvShowsViewModel;
    void loadFonts();
    void initializeDatabase();
    std::string getPathToDatabaseFile() const;
    void initializeStorageInDatabase(const std::string& databaseFilePath);
    void initializeQmlEngine();
    void exitOnUiLoadFailure(const QUrl &url);
    void registerViewModelsInEngine();
    void registerQmlTypes();
};

}

#endif //SERIO_APPLICATION_H