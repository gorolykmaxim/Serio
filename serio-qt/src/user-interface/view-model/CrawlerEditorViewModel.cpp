#include "CrawlerEditorViewModel.h"
#include <QQmlContext>
#include <user-interface/ViewNames.h>

namespace serio::qt {

CrawlerEditorViewModel::CrawlerEditorViewModel(core::TvShowCrawlerEditor &editor, StackOfViews &stack)
    : editor(editor), stack(stack) {
    crawlerEditorActions << new ButtonModel("cancel", ActionType::BACK, {}, false);
    crawlerEditorActions << new ButtonModel("add step", ActionType::OPEN_NEW_CRAWLER_STEP_EDITOR);
    crawlerEditorActions << new ButtonModel("save", ActionType::SAVE_CRAWLER);
    crawlerEditorActions << new ButtonModel("preview", ActionType::PREVIEW_CRAWLER);
    crawlerEditorActions << new ButtonModel("help", ActionType::OPEN_CRAWLER_EDITOR_HELP);
}

void CrawlerEditorViewModel::initialize(ActionRouter &router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("crawlerEditorViewModel", this);
    router.registerAction(ActionType::OPEN_CRAWLER_EDITOR, [this] (const auto& args) { openCrawlerEditor(static_cast<core::CrawlerType>(args[0].toInt())); });
    router.registerAction(ActionType::OPEN_CRAWLER_EDITOR_HELP, [this] (const auto& args) { openHelp(); });
    router.registerAction(ActionType::LOAD_CRAWLER_STEPS, [this] (const auto& args) { loadCrawlerSteps(); });
    router.registerAction(ActionType::PREVIEW_CRAWLER, [this] (const auto& args) { openCrawlerPreview(); });
    router.registerAction(ActionType::SAVE_CRAWLER, [this] (const auto& args) { save(); });
}

QString CrawlerEditorViewModel::getCrawlerType() const {
    if (!crawlerType) {
        throw CrawlerEditorViewNotOpenedError();
    }
    if (crawlerType == core::CrawlerType::episodeVideoCrawler) {
        return "Episode Video";
    } else if (crawlerType == core::CrawlerType::thumbnailCrawler) {
        return "Thumbnail";
    } else {
        return "Episode Name";
    }
}

QList<TileModel*> CrawlerEditorViewModel::getCrawlerSteps() const {
    return crawlerSteps;
}

int CrawlerEditorViewModel::getEpisodeVideoCrawlerType() const {
    return core::CrawlerType::episodeVideoCrawler;
}

int CrawlerEditorViewModel::getThumbnailCrawlerType() const {
    return core::CrawlerType::thumbnailCrawler;
}

int CrawlerEditorViewModel::getEpisodeNameCrawlerType() const {
    return core::CrawlerType::episodeNameCrawler;
}

QList<TileModel*> CrawlerEditorViewModel::getPreviewResults() const {
    return previewResults;
}

void CrawlerEditorViewModel::openCrawlerEditor(core::CrawlerType type) {
    editor.editCrawler(type);
    modifyModel([this, type] {
        crawlerType = type;
        emit crawlerTypeChanged();
        stack.pushView(crawlerEditorView);
    });
}

void CrawlerEditorViewModel::openHelp() {
    stack.pushView(crawlerEditorHelpView);
}

void CrawlerEditorViewModel::loadCrawlerSteps() {
    auto steps = editor.getCrawlerSteps();
    modifyModel([this, steps] { setCrawlerSteps(steps); });
}

void CrawlerEditorViewModel::save() {
    editor.saveCrawler();
    stack.popCurrentView();
}

void CrawlerEditorViewModel::openCrawlerPreview() {
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

void CrawlerEditorViewModel::setCrawlerSteps(const std::vector<core::CrawlerStep>& steps) {
    crawlerSteps.clearAndDelete();
    for (const auto& step: steps) {
        crawlerSteps << createTileFrom(step);
    }
    emit crawlerStepsChanged();
}

void CrawlerEditorViewModel::setPreviewResults(const std::vector<std::string> &results) {
    previewResults.clearAndDelete();
    for (const auto& result: results) {
        previewResults << new TileModel(QString::fromStdString(result));
    }
    emit previewResultsChanged();
}

TileModel *CrawlerEditorViewModel::createTileFrom(const core::CrawlerStep &step) const {
    auto title = QString::fromStdString(step.getType() + " step");
    title[0] = title[0].toUpper();
    auto subtitle = QString::fromStdString(step.getPropertiesAsString());
    return new TileModel(title, subtitle);
}

QList<ButtonModel*> CrawlerEditorViewModel::getCrawlerEditorActions() const {
    return crawlerEditorActions;
}

QList<ButtonModel*> CrawlerEditorViewModel::getCrawlerPreviewActions() const {
    return crawlerPreviewActions;
}

void CrawlerEditorViewModel::populateCrawlerPreviewActions() {
    crawlerPreviewActions.clearAndDelete();
    emit crawlerPreviewActionsChanged();
    crawlerPreviewActions << new ButtonModel("back", ActionType::BACK, {}, false);
    crawlerPreviewActions << new ButtonModel("view log", ActionType::OPEN_PREVIEWED_CRAWLER_LOG,
                                                        QVariantList({getCrawlerType()}));
    emit crawlerPreviewActionsChanged();
}

CrawlerEditorViewNotOpenedError::CrawlerEditorViewNotOpenedError()
    : std::logic_error("Call openCrawlerEditor() before trying to access crawler type") {}

}