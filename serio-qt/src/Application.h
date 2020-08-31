#ifndef SERIO_APPLICATION_H
#define SERIO_APPLICATION_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <storage/DatabaseStorage.h>
#include <concurrency/TaskExecutor.h>
#include <user-interface/UserInterface.h>

namespace serio::qt {

class Application : public QGuiApplication {
public:
    Application(int &argc, char **argv);
private:
    TaskExecutor executor;
    DatabaseStorage storage;
    UserInterface userInterface;
    std::string getPathToDatabaseFile() const;
    void initializeStorageInDatabase(const std::string& databaseFilePath);
};

}

#endif //SERIO_APPLICATION_H
