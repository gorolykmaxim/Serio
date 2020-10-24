#include "TvShowCrawlerEditorViewModel.h"
#include <QQmlContext>
#include <user-interface/ViewNames.h>

namespace serio::qt {

TvShowCrawlerEditorViewModel::TvShowCrawlerEditorViewModel(core::TvShowCrawlerEditor &editor, core::TvShowViewer& viewer,
                                                           DialogViewModel& dialog, StackOfViews &stack)
    : editor(editor), viewer(viewer), dialog(dialog), stack(stack) {
    importTvShowCrawlerActions << new ButtonModel("cancel", ActionType::BACK, {}, false);
    importTvShowCrawlerActions << new ButtonModel("import", ActionType::IMPORT_TV_SHOW_CRAWLER);
}

void TvShowCrawlerEditorViewModel::initialize(ActionRouter &router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("tvShowCrawlerEditorViewModel", this);
    router.registerAction(ActionType::SET_TV_SHOW_NAME, [this] (const auto& args) { setTvShowName(args); });
    router.registerAction(ActionType::SET_RAW_TV_SHOW_CRAWLER_TO_IMPORT, [this] (const auto& args) { setRawCrawlerToImport(args[0].toString()); });
    router.registerAction(ActionType::IMPORT_TV_SHOW_CRAWLER, [this] (const auto& args) { importTvShowCrawler(); });
    router.registerAction(ActionType::OPEN_ADD_TV_SHOW_VIEW, [this] (const auto& args) { openAddTvShowView(); });
    router.registerAction(ActionType::OPEN_TV_SHOW_CRAWLER_EDITOR_VIEW, [this] (const auto& args) { openTvShowCrawlerEditorView(); });
    router.registerAction(ActionType::OPEN_IMPORT_TV_SHOW_VIEW, [this] (const auto& args) { openImportTvShowCrawlerView(); });
    router.registerAction(ActionType::LOAD_TV_SHOW_CRAWLER_EDITOR_TV_SHOW_NAME, [this] (const auto& args) { loadTvShowName(); });
    router.registerAction(ActionType::SAVE_TV_SHOW_CRAWLER, [this] (const auto& args) { save(); });
    router.registerAction(ActionType::SAVE_TV_SHOW_CRAWLER_WITH_OVERRIDE, [this] (const auto& args) { saveWithOverride(); });
    router.registerAction(ActionType::OPEN_CURRENT_TV_SHOW_CRAWLER_EDITOR_VIEW, [this] (const auto& args) { openCurrentTvShowCrawlerEditorView(); });
}

QString TvShowCrawlerEditorViewModel::getTvShowName() const {
    return tvShowName;
}

bool TvShowCrawlerEditorViewModel::canCrawlerBeSaved() const {
    return !tvShowName.isEmpty();
}

bool TvShowCrawlerEditorViewModel::canTvShowNameBeChanged() const {
    return !isEditingExistingTvShow;
}

void TvShowCrawlerEditorViewModel::setTvShowName(const QVariantList& args) {
    auto name = args[0].toString();
    editor.setTvShowName(name.toStdString());
    modifyModel([name, this] { setName(name); });
}

void TvShowCrawlerEditorViewModel::openAddTvShowView() {
    stack.pushView(addTvShowView);
}

void TvShowCrawlerEditorViewModel::openTvShowCrawlerEditorView() {
    editor.createTvShowCrawler();
    openNewTvShowCrawlerEditorView(tvShowCrawlerEditorView);
}

void TvShowCrawlerEditorViewModel::openImportTvShowCrawlerView() {
    openNewTvShowCrawlerEditorView(importTvShowCrawlerView);
}

void TvShowCrawlerEditorViewModel::loadTvShowName() {
    auto name = QString::fromStdString(editor.getTvShowName());
    modifyModel([name, this] { setName(name); });
}

void TvShowCrawlerEditorViewModel::openCurrentTvShowCrawlerEditorView() {
    auto currentTvShow = viewer.getSelectedTvShow();
    editor.editTvShowCrawler(currentTvShow.getName());
    openExistingTvShowCrawlerEditorView();
}

void TvShowCrawlerEditorViewModel::save() {
    if (!isEditingExistingTvShow && editor.willOverrideExistingTvShow()) {
        displayTvShowOverrideConfirmationDialog();
    } else {
        saveWithOverride();
    }
}

void TvShowCrawlerEditorViewModel::saveWithOverride() {
    try {
        stack.pushView(crawlingInProgressView);
        auto name = editor.getTvShowName();
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

void TvShowCrawlerEditorViewModel::setName(QString name) {
    tvShowName = std::move(name);
    emit tvShowNameChanged();
    emit canCrawlerBeSavedChanged();
}

void TvShowCrawlerEditorViewModel::openNewTvShowCrawlerEditorView(const QString& view) {
    modifyModel([this, view] {
        rootEditorView = view;
        rawCrawlerToImport.clear();
        setEditingExistingTvShow(false);
        stack.replaceCurrentViewWith(rootEditorView);
    });
}

void TvShowCrawlerEditorViewModel::openExistingTvShowCrawlerEditorView() {
    modifyModel([this] {
        rootEditorView = tvShowCrawlerEditorView;
        setEditingExistingTvShow(true);
        stack.pushView(rootEditorView);
    });
}

void TvShowCrawlerEditorViewModel::setRawCrawlerToImport(QString rawCrawler) {
    rawCrawlerToImport = std::move(rawCrawler);
}

void TvShowCrawlerEditorViewModel::importTvShowCrawler() {
    editor.importTvShowCrawler(rawCrawlerToImport.toStdString());
    loadTvShowName();
    save();
}

void TvShowCrawlerEditorViewModel::setEditingExistingTvShow(bool editingExistingTvShow) {
    isEditingExistingTvShow = editingExistingTvShow;
    emit canTvShowNameBeChangedChanged();
}

void TvShowCrawlerEditorViewModel::displayTvShowOverrideConfirmationDialog() {
    DialogModel dialogModel("TV Show Already Exists",
                            "Are you sure you want to override crawler of the '" + tvShowName + "' with the new one?");
    dialogModel.setTopButtonAction(ActionType::SAVE_TV_SHOW_CRAWLER_WITH_OVERRIDE);
    dialog.display(dialogModel);
}

QList<ButtonModel*> TvShowCrawlerEditorViewModel::getImportTvShowCrawlerActions() const {
    return importTvShowCrawlerActions;
}

}