#include "TvShowCrawlerEditorViewModel.h"
#include <QQmlContext>
#include <utility>

serio::qt::TvShowCrawlerEditorViewModel::TvShowCrawlerEditorViewModel(serio::core::TvShowCrawlerEditor &editor,
                                                                      serio::qt::StackOfViews &stack)
    : editor(editor), stack(stack) {}

void serio::qt::TvShowCrawlerEditorViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("tvShowCrawlerEditorViewModel", this);
    router.registerAction(ActionType::SET_TV_SHOW_NAME, [this] (const QVariantList& args) { setTvShowName(args); });
    router.registerAction(ActionType::IMPORT_TV_SHOW_CRAWLER, [this] (const QVariantList& args) { importTvShowCrawler(args); });
    router.registerAction(ActionType::OPEN_ADD_TV_SHOW_VIEW, [this] (const QVariantList& args) { openAddTvShowView(); });
    router.registerAction(ActionType::OPEN_TV_SHOW_CRAWLER_EDITOR_VIEW, [this] (const QVariantList& args) { openTvShowCrawlerEditorView(); });
    router.registerAction(ActionType::OPEN_IMPORT_TV_SHOW_VIEW, [this] (const QVariantList& args) { openImportTvShowCrawlerView(); });
    router.registerAction(ActionType::LOAD_TV_SHOW_CRAWLER_EDITOR_TV_SHOW_NAME, [this] (const QVariantList& args) { loadTvShowName(); });
    router.registerAction(ActionType::SAVE_TV_SHOW_CRAWLER, [this] (const QVariantList& args) { save(); });
    router.registerAction(ActionType::SAVE_TV_SHOW_CRAWLER_WITH_OVERRIDE, [this] (const QVariantList& args) { saveWithOverride(); });
}

QString serio::qt::TvShowCrawlerEditorViewModel::getTvShowName() const {
    return tvShowName;
}

bool serio::qt::TvShowCrawlerEditorViewModel::canCrawlerBeSaved() const {
    return !tvShowName.isEmpty();
}

void serio::qt::TvShowCrawlerEditorViewModel::setTvShowName(const QVariantList& args) {
    QString name = args[0].toString();
    editor.setTvShowName(name.toStdString());
    modifyModel([name, this] { setName(name); });
}

void serio::qt::TvShowCrawlerEditorViewModel::openAddTvShowView() {
    stack.pushView(QString("AddTvShowView.qml"));
}

void serio::qt::TvShowCrawlerEditorViewModel::openTvShowCrawlerEditorView() {
    editor.createTvShowCrawler();
    stack.replaceCurrentViewWith(QString("TvShowCrawlerEditorView.qml"));
}

void serio::qt::TvShowCrawlerEditorViewModel::openImportTvShowCrawlerView() {
    stack.replaceCurrentViewWith(QString("ImportTvShowCrawlerView.qml"));
}

void serio::qt::TvShowCrawlerEditorViewModel::loadTvShowName() {
    QString name = QString::fromStdString(editor.getTvShowName());
    modifyModel([name, this] { setName(name); });
}

void serio::qt::TvShowCrawlerEditorViewModel::save() {
    if (editor.willOverrideExistingTvShow()) {
        stack.pushView("TvShowCrawlerOverrideDialogView.qml");
    } else {
        saveWithOverride();
    }
}

void serio::qt::TvShowCrawlerEditorViewModel::saveWithOverride() {
    try {
        stack.pushView("CrawlingInProgressView.qml");
        editor.saveAndRunTvShowCrawler();
        stack.popAllViews();
    } catch (std::runtime_error& e) {
        stack.popCurrentView();
        throw e;
    }
}

void serio::qt::TvShowCrawlerEditorViewModel::setName(QString name) {
    tvShowName = std::move(name);
    emit tvShowNameChanged();
    emit canCrawlerBeSavedChanged();
}

void serio::qt::TvShowCrawlerEditorViewModel::importTvShowCrawler(const QVariantList &args) {
    editor.importTvShowCrawler(args[0].toString().toStdString());
    loadTvShowName();
    save();
}
