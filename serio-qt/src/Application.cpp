#include "Application.h"

namespace serio::qt {

Application::Application(int &argc, char **argv)
    : QGuiApplication(argc, argv),
      tvShowCrawlerRuntime(storage, storage, storage, httpClient),
      tvShowCrawlerEditor(tvShowCrawlerRuntime),
      tvShowPlayer(storage),
      viewer(storage, storage, tvShowCrawlerRuntime),
      userInterface(httpClient, tvShowCrawlerEditor, storage, viewer, tvShowPlayer, executor) {
    executor.runInBackgroundAndWait([this] { storage.initialize(); });
    userInterface.initialize();
}

}