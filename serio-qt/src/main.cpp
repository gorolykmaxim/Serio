#include "Application.h"

int main(int argc, char* argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    serio::qt::Application app(argc, argv);
    return app.exec();
}