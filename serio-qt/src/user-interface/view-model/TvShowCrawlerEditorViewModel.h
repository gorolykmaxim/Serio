#ifndef SERIO_TVSHOWCRAWLEREDITORVIEWMODEL_H
#define SERIO_TVSHOWCRAWLEREDITORVIEWMODEL_H

#include <QFutureWatcher>
#include <tv-show-crawler-editor/TvShowCrawlerEditor.h>
#include <task-executor/QTaskExecutor.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/model/TileModel.h>
#include <QQmlApplicationEngine>
#include <user-interface/action/ActionRouter.h>
#include "ViewModel.h"

namespace serio::qt {

class TvShowCrawlerEditorViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(QString tvShowName READ getTvShowName NOTIFY tvShowNameChanged)
    Q_PROPERTY(bool canCrawlerBeSaved READ canCrawlerBeSaved NOTIFY canCrawlerBeSavedChanged)
public:
    TvShowCrawlerEditorViewModel(serio::core::TvShowCrawlerEditor &editor, StackOfViews &stack);
    void initialize(ActionRouter& router, QQmlApplicationEngine& engine);
    [[nodiscard]] QString getTvShowName() const;
    [[nodiscard]] bool canCrawlerBeSaved() const;
    void openAddTvShowView();
    void openTvShowCrawlerEditorView();
    void setTvShowName(const QVariantList& args);
    void importTvShowCrawler(const QVariantList &args);
    void openImportTvShowCrawlerView();
    void loadTvShowName();
    void save();
    void saveWithOverride();
signals:
    void tvShowNameChanged();
    void canCrawlerBeSavedChanged();
private:
    QString tvShowName;
    QString rootEditorView;
    core::TvShowCrawlerEditor& editor;
    StackOfViews& stack;
    void setName(QString name);
    void openEditorView(QString view);
};

}


#endif //SERIO_TVSHOWCRAWLEREDITORVIEWMODEL_H
