#include "CrawlerEditorViewModel.h"
#include <QQmlContext>
#include <user-interface/ViewNames.h>

serio::qt::CrawlerEditorViewModel::CrawlerEditorViewModel(serio::core::TvShowCrawlerEditor &editor,
                                                          serio::qt::StackOfViews &stack)
    : editor(editor), stack(stack) {
    crawlerEditorActions << new serio::qt::ButtonModel("cancel", serio::qt::ActionType::BACK, {}, false);
    crawlerEditorActions << new serio::qt::ButtonModel("add step", serio::qt::ActionType::OPEN_NEW_CRAWLER_STEP_EDITOR);
    crawlerEditorActions << new serio::qt::ButtonModel("save", serio::qt::ActionType::SAVE_CRAWLER);
    crawlerEditorActions << new serio::qt::ButtonModel("preview", serio::qt::ActionType::PREVIEW_CRAWLER);
    crawlerEditorActions << new serio::qt::ButtonModel("help", serio::qt::ActionType::OPEN_CRAWLER_EDITOR_HELP);
}

void serio::qt::CrawlerEditorViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("crawlerEditorViewModel", this);
    router.registerAction(ActionType::OPEN_CRAWLER_EDITOR, [this] (const auto& args) { openCrawlerEditor(static_cast<core::CrawlerType>(args[0].toInt())); });
    router.registerAction(ActionType::OPEN_CRAWLER_EDITOR_HELP, [this] (const auto& args) { openHelp(); });
    router.registerAction(ActionType::LOAD_CRAWLER_STEPS, [this] (const auto& args) { loadCrawlerSteps(); });
    router.registerAction(ActionType::PREVIEW_CRAWLER, [this] (const auto& args) { openCrawlerPreview(); });
    router.registerAction(ActionType::SAVE_CRAWLER, [this] (const auto& args) { save(); });
}

QString serio::qt::CrawlerEditorViewModel::getCrawlerType() const {
    if (!crawlerType) {
        throw CrawlerEditorViewNotOpenedError();
    }
    if (crawlerType == serio::core::CrawlerType::episodeVideoCrawler) {
        return "Episode Video";
    } else if (crawlerType == serio::core::CrawlerType::thumbnailCrawler) {
        return "Thumbnail";
    } else {
        return "Episode Name";
    }
}

QList<serio::qt::TileModel*> serio::qt::CrawlerEditorViewModel::getCrawlerSteps() const {
    return crawlerSteps;
}

int serio::qt::CrawlerEditorViewModel::getEpisodeVideoCrawlerType() const {
    return core::CrawlerType::episodeVideoCrawler;
}

int serio::qt::CrawlerEditorViewModel::getThumbnailCrawlerType() const {
    return core::CrawlerType::thumbnailCrawler;
}

int serio::qt::CrawlerEditorViewModel::getEpisodeNameCrawlerType() const {
    return core::CrawlerType::episodeNameCrawler;
}

QList<serio::qt::TileModel*> serio::qt::CrawlerEditorViewModel::getPreviewResults() const {
    return previewResults;
}

void serio::qt::CrawlerEditorViewModel::openCrawlerEditor(serio::core::CrawlerType type) {
    editor.editCrawler(type);
    modifyModel([this, type] {
        crawlerType = type;
        emit crawlerTypeChanged();
        stack.pushView(crawlerEditorView);
    });
}

void serio::qt::CrawlerEditorViewModel::openHelp() {
    stack.pushView(crawlerEditorHelpView);
}

void serio::qt::CrawlerEditorViewModel::loadCrawlerSteps() {
    auto steps = editor.getCrawlerSteps();
    modifyModel([this, steps] { setCrawlerSteps(steps); });
}

void serio::qt::CrawlerEditorViewModel::save() {
    editor.saveCrawler();
    stack.popCurrentView();
}

void serio::qt::CrawlerEditorViewModel::openCrawlerPreview() {
    try {
        stack.pushView(crawlingInProgressView);
        auto results = editor.previewCrawler().result;
        modifyModel([this, results] {
            populateCrawlerPreviewActions();
            setPreviewResults(results);
            stack.replaceCurrentViewWith(crawlerPreviewView);
        });
    } catch (std::runtime_error& e) {
        stack.popCurrentView();
        throw e;
    }
}

void serio::qt::CrawlerEditorViewModel::setCrawlerSteps(const std::vector<core::CrawlerStep>& steps) {
    crawlerSteps.clearAndDelete();
    for (const auto& step: steps) {
        crawlerSteps << createTileFrom(step);
    }
    emit crawlerStepsChanged();
}

void serio::qt::CrawlerEditorViewModel::setPreviewResults(const std::vector<std::string> &results) {
    previewResults.clearAndDelete();
    for (const auto& result: results) {
        previewResults << new TileModel(QString::fromStdString(result));
    }
    emit previewResultsChanged();
}

serio::qt::TileModel *serio::qt::CrawlerEditorViewModel::createTileFrom(const serio::core::CrawlerStep &step) const {
    auto title = QString::fromStdString(step.getType() + " step");
    title[0] = title[0].toUpper();
    auto subtitle = QString::fromStdString(step.getPropertiesAsString());
    return new TileModel(title, subtitle);
}

QList<serio::qt::ButtonModel*> serio::qt::CrawlerEditorViewModel::getCrawlerEditorActions() const {
    return crawlerEditorActions;
}

QList<serio::qt::ButtonModel*> serio::qt::CrawlerEditorViewModel::getCrawlerPreviewActions() const {
    return crawlerPreviewActions;
}

void serio::qt::CrawlerEditorViewModel::populateCrawlerPreviewActions() {
    crawlerPreviewActions.clearAndDelete();
    emit crawlerPreviewActionsChanged();
    crawlerPreviewActions << new serio::qt::ButtonModel("back", serio::qt::ActionType::BACK, {}, false);
    crawlerPreviewActions << new serio::qt::ButtonModel("view log", serio::qt::ActionType::OPEN_PREVIEWED_CRAWLER_LOG,
                                                        QVariantList({getCrawlerType()}));
    emit crawlerPreviewActionsChanged();
}

serio::qt::CrawlerEditorViewNotOpenedError::CrawlerEditorViewNotOpenedError()
    : std::logic_error("Call openCrawlerEditor() before trying to access crawler type") {}
