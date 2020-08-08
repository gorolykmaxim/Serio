#include <QDir>
#include <QFontDatabase>
#include <QQmlContext>
#include <HelloWorld.h>
#include "Application.h"

serio::Application::Application(int &argc, char **argv) : QGuiApplication(argc, argv) {
    loadFonts();
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    exitOnUiLoadFailure(url);
    engine.rootContext()->setContextProperty("helloWorld", QString::fromStdString(core::getHelloWorld()));
    engine.load(url);
}

void serio::Application::loadFonts() {
    QString fontsDir = ":/fonts/";
    for (const QString& font: QDir(fontsDir).entryList()) {
        QFontDatabase::addApplicationFont(fontsDir + font);
    }
}

void serio::Application::exitOnUiLoadFailure(const QUrl &url) {
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, this, [url] (QObject* obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            QCoreApplication::exit(1);
        }
    }, Qt::QueuedConnection);
}
