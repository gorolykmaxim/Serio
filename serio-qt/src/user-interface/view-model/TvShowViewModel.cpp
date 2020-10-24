#include "TvShowViewModel.h"
#include <QQmlContext>
#include <QGuiApplication>
#include <QClipboard>
#include <user-interface/ViewNames.h>

namespace serio::qt {

TvShowViewModel::TvShowViewModel(unsigned int pageSize, unsigned int pageCountLimit, core::TvShowViewer &viewer,
                                 DialogViewModel& dialog, BackgroundViewModel& background, SnackbarViewModel& snackbar,
                                 StackOfViews& stack)
    : episodeListModel(pageSize, pageCountLimit),
      viewer(viewer),
      dialog(dialog),
      background(background),
      snackbar(snackbar),
      stack(stack) {
    populateActions(false);
}

void TvShowViewModel::initialize(ActionRouter &router, QQmlApplicationEngine &engine) {
    qmlRegisterUncreatableType<EpisodeListModel>("Serio", 1, 0, "EpisodeListModel", nullptr);
    engine.rootContext()->setContextProperty("tvShowViewModel", this);
    router.registerAction(ActionType::LOAD_TV_SHOW, [this] (const auto& args) { load(); });
    router.registerAction(ActionType::LOAD_EPISODES_LIST_PAGE, [this] (const auto& args) { loadEpisodes(args); });
    router.registerAction(ActionType::SHARE_CRAWLER_OF_CURRENT_TV_SHOW, [this] (const auto& args) { shareCrawler(); });
    router.registerAction(ActionType::CRAWL_CURRENT_TV_SHOW, [this] (const auto& args) { crawl(); });
    router.registerAction(ActionType::CONFIRM_CLEAR_CURRENT_TV_SHOW_WATCH_HISTORY, [this] (const auto& args) { confirmClearWatchHistory(); });
    router.registerAction(ActionType::CLEAR_CURRENT_TV_SHOW_WATCH_HISTORY, [this] (const auto& args) { clearWatchHistory(); });
    router.registerAction(ActionType::CONFIRM_DELETE_CURRENT_TV_SHOW, [this] (const auto& args) { confirmDeleteTvShow(); });
    router.registerAction(ActionType::DELETE_CURRENT_TV_SHOW, [this] (const auto& args) { deleteTvShow(); });
    router.registerAction(ActionType::OPEN_TV_SHOW_DETAILS_VIEW, [this] (const auto& args) { openTvShowDetails(); });
    connect(&episodeListModel, &EpisodeListModel::requestPageLoad,
            this, [&router] (auto offset, auto limit) { router.trigger(ActionType::LOAD_EPISODES_LIST_PAGE, QVariantList({offset, limit})); });
}

QString TvShowViewModel::getTvShowName() const {
    return tvShowName;
}

QString TvShowViewModel::getLastWatchDate() const {
    return lastWatchDate;
}

QString TvShowViewModel::getThumbnailUrl() const {
    return thumbnailUrl;
}

EpisodeListModel* TvShowViewModel::getEpisodeList() {
    return &episodeListModel;
}

void TvShowViewModel::load() {
    loadTvShow();
    modifyModel([this] { episodeListModel.requestFirstPageLoad(); });
}

void TvShowViewModel::loadEpisodes(const QVariantList& args) {
    auto episodes = viewer.getTvShowEpisodes(args[0].toUInt(),args[1].toUInt());
    modifyModel([this, episodes] {
        populateActions(episodes.getTotalSize() > 0);
        episodeListModel.loadPage(episodes);
    });
}

void TvShowViewModel::shareCrawler() {
    auto rawTvShowCrawler = viewer.getRawCrawlerOfSelectedTvShow();
    modifyModel([this, rawTvShowCrawler] {
        QGuiApplication::clipboard()->setText(QString::fromStdString(rawTvShowCrawler));
        snackbar.displayText("Crawler copied to your clipboard");
    });
}

void TvShowViewModel::loadTvShow() {
    auto tvShow = viewer.getSelectedTvShow();
    modifyModel([this, tvShow] {
        tvShowName = QString::fromStdString(tvShow.getName());
        thumbnailUrl = QString::fromStdString(tvShow.getThumbnailUrl());
        background.setImage(thumbnailUrl);
        auto date = tvShow.getLastWatchDate();
        lastWatchDate = QString::fromStdString(date ? "Last watched " + date->toString() : "");
        emit selectedTvShowChanged();
    });
}

void TvShowViewModel::crawl() {
    try {
        stack.pushView(crawlingInProgressView);
        viewer.crawlSelectedTvShow();
        stack.popCurrentView();
    } catch (std::runtime_error& e) {
        stack.popCurrentView();
        throw e;
    }
}

void TvShowViewModel::confirmClearWatchHistory() {
    DialogModel model("Clear Watch History",
                      "You are about to clear your watch history of '" + tvShowName + "'.");
    model.setTopButtonAction(ActionType::CLEAR_CURRENT_TV_SHOW_WATCH_HISTORY);
    dialog.display(model);
}

void TvShowViewModel::clearWatchHistory() {
    viewer.clearSelectedTvShowWatchHistory();
    stack.popCurrentView();
    snackbar.displayText("Watch history cleared");
}

void TvShowViewModel::confirmDeleteTvShow() {
    DialogModel model("Delete TV Show",
                      "You are about to delete '" + tvShowName + "'");
    model.setTopButtonAction(ActionType::DELETE_CURRENT_TV_SHOW);
    dialog.display(model);
}

void TvShowViewModel::deleteTvShow() {
    viewer.deleteSelectedTvShow();
    stack.popAllViews();
}

void TvShowViewModel::openTvShowDetails() {
    stack.pushView(tvShowDetailsView);
}

QList<ButtonModel*> TvShowViewModel::getActions() const {
    return actions;
}

void TvShowViewModel::populateActions(bool includePlayAction) {
    actions.clearAndDelete();
    emit actionsChanged();
    if (includePlayAction) {
        actions << new ButtonModel(lastWatchDate.isEmpty() ? "play" : "resume", ActionType::PLAY_TV_SHOW, {tvShowName});
    }
    actions << new ButtonModel("back", ActionType::BACK, {}, false);
    actions << new ButtonModel("details", ActionType::OPEN_TV_SHOW_DETAILS_VIEW);
    emit actionsChanged();
}

}