#ifndef SERIO_TVSHOWCRAWLEREDITORVIEWMODEL_H
#define SERIO_TVSHOWCRAWLEREDITORVIEWMODEL_H

#include <tv-show-crawler-editor/TvShowCrawlerEditor.h>
#include <user-interface/StackOfViews.h>
#include <QQmlApplicationEngine>
#include <user-interface/action/ActionRouter.h>
#include <tv-show-viewer/TvShowViewer.h>
#include <user-interface/model/ButtonModel.h>
#include "ViewModel.h"
#include "DialogViewModel.h"
#include <user-interface/model/ListModel.h>

namespace serio::qt {

class TvShowCrawlerEditorViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(QString tvShowName READ getTvShowName NOTIFY tvShowNameChanged)
    Q_PROPERTY(bool canCrawlerBeSaved READ canCrawlerBeSaved NOTIFY canCrawlerBeSavedChanged)
    Q_PROPERTY(bool canTvShowNameBeChanged READ canTvShowNameBeChanged NOTIFY canTvShowNameBeChangedChanged)
    Q_PROPERTY(QList<ButtonModel*> importTvShowCrawlerActions READ getImportTvShowCrawlerActions CONSTANT)
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
    void setRawCrawlerToImport(QString rawCrawler);
    void importTvShowCrawler();
    void openImportTvShowCrawlerView();
    void openCurrentTvShowCrawlerEditorView();
    void loadTvShowName();
    void save();
    void saveWithOverride();
    [[nodiscard]] QList<ButtonModel*> getImportTvShowCrawlerActions() const;
signals:
    void tvShowNameChanged();
    void canCrawlerBeSavedChanged();
    void canTvShowNameBeChangedChanged();
private:
    bool isEditingExistingTvShow;
    QString tvShowName;
    QString rootEditorView;
    QString rawCrawlerToImport;
    ListModel<ButtonModel*> importTvShowCrawlerActions;
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
