#ifndef SERIO_USERINTERFACE_H
#define SERIO_USERINTERFACE_H

#include <QQmlApplicationEngine>
#include <user-interface/view-model/AllTvShowsViewModel.h>
#include <database-storage/DatabaseStorage.h>

namespace serio::qt {

class UserInterface : public QObject {
    Q_OBJECT
public:
    UserInterface(DatabaseStorage& storage, QTaskExecutor& executor);
    void initialize();
private:
    QQmlApplicationEngine engine;
    AllTvShowsViewModel allTvShowsViewModel;
    void loadFonts();
    void exitOnUiLoadFailure(const QUrl &url);
    void registerViewModelsInEngine();
    void registerQmlTypes();
};

}

#endif //SERIO_USERINTERFACE_H
