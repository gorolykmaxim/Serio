#include "Application.h"

serio::qt::Application::Application(int &argc, char **argv)
    : QGuiApplication(argc, argv),
      tvShowCrawlerRuntime(storage, storage, httpClient),
      tvShowCrawlerEditor(tvShowCrawlerRuntime),
      userInterface(httpClient, tvShowCrawlerEditor, storage, executor) {
    executor.runInBackgroundAndWait([this] { storage.initialize(); });
    userInterface.initialize();
}
