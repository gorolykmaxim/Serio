#include "Application.h"

serio::qt::Application::Application(int &argc, char **argv)
    : QGuiApplication(argc, argv),
      tvShowCrawlerRuntime(storage, storage, httpClient),
      tvShowCrawlerEditor(tvShowCrawlerRuntime),
      viewer(storage),
      userInterface(httpClient, tvShowCrawlerEditor, storage, viewer, executor) {
    executor.runInBackgroundAndWait([this] { storage.initialize(); });
    userInterface.initialize();
}
