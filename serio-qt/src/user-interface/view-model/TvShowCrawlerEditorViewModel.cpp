#include "TvShowCrawlerEditorViewModel.h"
#include <QQmlContext>
#include <user-interface/ViewNames.h>

serio::qt::TvShowCrawlerEditorViewModel::TvShowCrawlerEditorViewModel(serio::core::TvShowCrawlerEditor &editor,
                                                                      serio::core::TvShowViewer& viewer,
                                                                      serio::qt::DialogViewModel& dialog,
                                                                      serio::qt::StackOfViews &stack)
    : editor(editor), viewer(viewer), dialog(dialog), stack(stack) {
    importTvShowCrawlerActions << new serio::qt::ButtonModel("cancel", serio::qt::ActionType::BACK);
    importTvShowCrawlerActions << new serio::qt::ButtonModel("import", serio::qt::ActionType::IMPORT_TV_SHOW_CRAWLER);
}

void serio::qt::TvShowCrawlerEditorViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("tvShowCrawlerEditorViewModel", this);
    router.registerAction(ActionType::SET_TV_SHOW_NAME, [this] (const QVariantList& args) { setTvShowName(args); });
    router.registerAction(ActionType::SET_RAW_TV_SHOW_CRAWLER_TO_IMPORT, [this] (const QVariantList& args) { setRawCrawlerToImport(args[0].toString()); });
    router.registerAction(ActionType::IMPORT_TV_SHOW_CRAWLER, [this] (const QVariantList& args) { importTvShowCrawler(); });
    router.registerAction(ActionType::OPEN_ADD_TV_SHOW_VIEW, [this] (const QVariantList& args) { openAddTvShowView(); });
    router.registerAction(ActionType::OPEN_TV_SHOW_CRAWLER_EDITOR_VIEW, [this] (const QVariantList& args) { openTvShowCrawlerEditorView(); });
    router.registerAction(ActionType::OPEN_IMPORT_TV_SHOW_VIEW, [this] (const QVariantList& args) { openImportTvShowCrawlerView(); });
    router.registerAction(ActionType::LOAD_TV_SHOW_CRAWLER_EDITOR_TV_SHOW_NAME, [this] (const QVariantList& args) { loadTvShowName(); });
    router.registerAction(ActionType::SAVE_TV_SHOW_CRAWLER, [this] (const QVariantList& args) { save(); });
    router.registerAction(ActionType::SAVE_TV_SHOW_CRAWLER_WITH_OVERRIDE, [this] (const QVariantList& args) { saveWithOverride(); });
    router.registerAction(ActionType::OPEN_CURRENT_TV_SHOW_CRAWLER_EDITOR_VIEW, [this] (const QVariantList& args) { openCurrentTvShowCrawlerEditorView(); });
}

QString serio::qt::TvShowCrawlerEditorViewModel::getTvShowName() const {
    return tvShowName;
}

bool serio::qt::TvShowCrawlerEditorViewModel::canCrawlerBeSaved() const {
    return !tvShowName.isEmpty();
}

bool serio::qt::TvShowCrawlerEditorViewModel::canTvShowNameBeChanged() const {
    return !isEditingExistingTvShow;
}

void serio::qt::TvShowCrawlerEditorViewModel::setTvShowName(const QVariantList& args) {
    QString name = args[0].toString();
    editor.setTvShowName(name.toStdString());
    modifyModel([name, this] { setName(name); });
}

void serio::qt::TvShowCrawlerEditorViewModel::openAddTvShowView() {
    stack.pushView(addTvShowView);
}

void serio::qt::TvShowCrawlerEditorViewModel::openTvShowCrawlerEditorView() {
    editor.createTvShowCrawler();
    openNewTvShowCrawlerEditorView(tvShowCrawlerEditorView);
}

void serio::qt::TvShowCrawlerEditorViewModel::openImportTvShowCrawlerView() {
    openNewTvShowCrawlerEditorView(importTvShowCrawlerView);
}

void serio::qt::TvShowCrawlerEditorViewModel::loadTvShowName() {
    QString name = QString::fromStdString(editor.getTvShowName());
    modifyModel([name, this] { setName(name); });
}

void serio::qt::TvShowCrawlerEditorViewModel::openCurrentTvShowCrawlerEditorView() {
    serio::core::TvShow currentTvShow = viewer.getSelectedTvShow();
    editor.editTvShowCrawler(currentTvShow.getName());
    openExistingTvShowCrawlerEditorView();
}

void serio::qt::TvShowCrawlerEditorViewModel::save() {
    if (!isEditingExistingTvShow && editor.willOverrideExistingTvShow()) {
        displayTvShowOverrideConfirmationDialog();
    } else {
        saveWithOverride();
    }
}

void serio::qt::TvShowCrawlerEditorViewModel::saveWithOverride() {
    try {
        stack.pushView(crawlingInProgressView);
        std::string name = editor.getTvShowName();
        editor.saveAndRunTvShowCrawler();
        if (isEditingExistingTvShow) {
            stack.popAllViewsUntil(tvShowView);
        } else {
            viewer.openTvShowWithName(name);
            stack.replaceSpecifiedViewWith(rootEditorView, tvShowView);
        }
    } catch (std::runtime_error& e) {
        stack.popAllViewsUntil(rootEditorView);
        throw e;
    }
}

void serio::qt::TvShowCrawlerEditorViewModel::setName(QString name) {
    tvShowName = std::move(name);
    emit tvShowNameChanged();
    emit canCrawlerBeSavedChanged();
}

void serio::qt::TvShowCrawlerEditorViewModel::openNewTvShowCrawlerEditorView(const QString& view) {
    modifyModel([this, view] {
        rootEditorView = view;
        rawCrawlerToImport.clear();
        setEditingExistingTvShow(false);
        stack.replaceCurrentViewWith(rootEditorView);
    });
}

void serio::qt::TvShowCrawlerEditorViewModel::openExistingTvShowCrawlerEditorView() {
    modifyModel([this] {
        rootEditorView = tvShowCrawlerEditorView;
        setEditingExistingTvShow(true);
        stack.pushView(rootEditorView);
    });
}

void serio::qt::TvShowCrawlerEditorViewModel::setRawCrawlerToImport(QString rawCrawler) {
    rawCrawlerToImport = std::move(rawCrawler);
}

void serio::qt::TvShowCrawlerEditorViewModel::importTvShowCrawler() {
    editor.importTvShowCrawler(rawCrawlerToImport.toStdString());
    loadTvShowName();
    save();
}

void serio::qt::TvShowCrawlerEditorViewModel::setEditingExistingTvShow(bool editingExistingTvShow) {
    isEditingExistingTvShow = editingExistingTvShow;
    emit canTvShowNameBeChangedChanged();
}

void serio::qt::TvShowCrawlerEditorViewModel::displayTvShowOverrideConfirmationDialog() {
    serio::qt::DialogModel dialogModel("TV Show Already Exists",
                                       "Are you sure you want to override crawler of the '" + tvShowName + "' with the new one?");
    dialogModel.setTopButtonAction(serio::qt::ActionType::SAVE_TV_SHOW_CRAWLER_WITH_OVERRIDE);
    dialog.display(dialogModel);
}

QList<serio::qt::ButtonModel*> serio::qt::TvShowCrawlerEditorViewModel::getImportTvShowCrawlerActions() const {
    return importTvShowCrawlerActions;
}
