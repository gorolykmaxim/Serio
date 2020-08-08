#ifndef SERIO_APPLICATION_H
#define SERIO_APPLICATION_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>

namespace serio {
    class Application : public QGuiApplication {
    public:
        Application(int &argc, char **argv);
    private:
        QQmlApplicationEngine engine;
        void loadFonts();
        void exitOnUiLoadFailure(const QUrl &url);
    };
}

#endif //SERIO_APPLICATION_H
