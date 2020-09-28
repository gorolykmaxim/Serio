#ifndef SERIO_TVSHOWCRAWLEREDITORVIEWMODEL_H
#define SERIO_TVSHOWCRAWLEREDITORVIEWMODEL_H

#include <QFutureWatcher>
#include <tv-show-crawler-editor/TvShowCrawlerEditor.h>
#include <task-executor/QTaskExecutor.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/model/TileModel.h>
#include <QQmlApplicationEngine>
#include <user-interface/action/ActionRouter.h>
#include <tv-show-viewer/TvShowViewer.h>
#include "ViewModel.h"
#include "DialogViewModel.h"

namespace serio::qt {

class TvShowCrawlerEditorViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(QString tvShowName READ getTvShowName NOTIFY tvShowNameChanged)
    Q_PROPERTY(bool canCrawlerBeSaved READ canCrawlerBeSaved NOTIFY canCrawlerBeSavedChanged)
    Q_PROPERTY(bool canTvShowNameBeChanged READ canTvShowNameBeChanged NOTIFY canTvShowNameBeChangedChanged)
public:
    TvShowCrawlerEditorViewModel(serio::core::TvShowCrawlerEditor &editor, core::TvShowViewer& viewer,
                                 DialogViewModel& dialog, StackOfViews &stack);
    void initialize(ActionRouter& router, QQmlApplicationEngine& engine);
    [[nodiscard]] QString getTvShowName() const;
    [[nodiscard]] bool canCrawlerBeSaved() const;
    [[nodiscard]] bool canTvShowNameBeChanged() const;
    void openAddTvShowView();
    void openTvShowCrawlerEditorView();
    void setTvShowName(const QVariantList& args);
    void importTvShowCrawler(const QVariantList &args);
    void openImportTvShowCrawlerView();
    void openCurrentTvShowCrawlerEditorView();
    void loadTvShowName();
    void save();
    void saveWithOverride();
signals:
    void tvShowNameChanged();
    void canCrawlerBeSavedChanged();
    void canTvShowNameBeChangedChanged();
private:
    bool isEditingExistingTvShow;
    QString tvShowName;
    QString rootEditorView;
    core::TvShowCrawlerEditor& editor;
    core::TvShowViewer& viewer;
    DialogViewModel& dialog;
    StackOfViews& stack;
    void setName(QString name);
    void openNewTvShowCrawlerEditorView(const QString& view);
    void openExistingTvShowCrawlerEditorView();
    void setEditingExistingTvShow(bool editingExistingTvShow);
    void displayTvShowOverrideConfirmationDialog();
};

}


#endif //SERIO_TVSHOWCRAWLEREDITORVIEWMODEL_H
