#include "CrawlerEditorViewModel.h"
#include <QQmlContext>
#include <sstream>

serio::qt::CrawlerEditorViewModel::CrawlerEditorViewModel(serio::core::TvShowCrawlerEditor &editor,
                                                          serio::qt::StackOfViews &stack)
    : editor(editor), stack(stack) {}

void serio::qt::CrawlerEditorViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("crawlerEditorViewModel", this);
    router.registerAction(ActionType::OPEN_CRAWLER_EDITOR, [this] (const QVariantList& args) { openCrawlerEditor(static_cast<core::CrawlerType>(args[0].toInt())); });
    router.registerAction(ActionType::OPEN_CRAWLER_EDITOR_HELP, [this] (const QVariantList& args) { openHelp(); });
    router.registerAction(ActionType::LOAD_CRAWLER_STEPS, [this] (const QVariantList& args) { loadCrawlerSteps(); });
    router.registerAction(ActionType::PREVIEW_CRAWLER, [this] (const QVariantList& args) { openCrawlerPreview(); });
    router.registerAction(ActionType::SAVE_CRAWLER, [this] (const QVariantList& args) { save(); });
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
        stack.pushView("CrawlerEditorView.qml");
    });
}

void serio::qt::CrawlerEditorViewModel::openHelp() {
    stack.pushView("CrawlerEditorHelpView.qml");
}

void serio::qt::CrawlerEditorViewModel::loadCrawlerSteps() {
    std::vector<core::CrawlerStep> steps = editor.getCrawlerSteps();
    modifyModel([this, steps] { setCrawlerSteps(steps); });
}

void serio::qt::CrawlerEditorViewModel::save() {
    editor.saveCrawler();
    stack.popCurrentView();
}

void serio::qt::CrawlerEditorViewModel::openCrawlerPreview() {
    try {
        stack.pushView("CrawlingInProgressView.qml");
        std::vector<std::string> results = editor.previewCrawler().result;
        stack.replaceCurrentViewWith("CrawlerPreviewView.qml");
        modifyModel([this, results] { setPreviewResults(results); });
    } catch (std::runtime_error& e) {
        stack.popCurrentView();
        throw e;
    }
}

void serio::qt::CrawlerEditorViewModel::setCrawlerSteps(const std::vector<core::CrawlerStep>& steps) {
    crawlerSteps.clearAndDelete();
    for (const core::CrawlerStep& step: steps) {
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
    QString title = QString::fromStdString(step.getType()) + " step";
    title[0] = title[0].toUpper();
    std::stringstream subtitleStream;
    for (const auto& entry: step.getProperties()) {
        subtitleStream << entry.first << ": " << entry.second << ", ";
    }
    QString subtitle = QString::fromStdString(subtitleStream.str()).remove(-2, 2);
    return new TileModel(title, subtitle);
}

serio::qt::CrawlerEditorViewNotOpenedError::CrawlerEditorViewNotOpenedError()
    : std::logic_error("Call openCrawlerEditor() before trying to access crawler type") {}
