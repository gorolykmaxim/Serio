#include "TvShowViewModel.h"
#include <QQmlContext>
#include <QGuiApplication>
#include <QClipboard>
#include <user-interface/ViewNames.h>

serio::qt::TvShowViewModel::TvShowViewModel(unsigned int pageSize, unsigned int pageCountLimit,
                                            serio::core::TvShowViewer &viewer, serio::qt::DialogViewModel& dialog,
                                            serio::qt::BackgroundViewModel& background,
                                            serio::qt::SnackbarViewModel& snackbar, StackOfViews& stack)
    : episodeListModel(pageSize, pageCountLimit),
      viewer(viewer),
      dialog(dialog),
      background(background),
      snackbar(snackbar),
      stack(stack) {
    actions << new serio::qt::ButtonModel("back", serio::qt::ActionType::BACK, {}, false);
    actions << new serio::qt::ButtonModel("details", serio::qt::ActionType::OPEN_TV_SHOW_DETAILS_VIEW);
}

void serio::qt::TvShowViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine &engine) {
    qmlRegisterUncreatableType<EpisodeListModel>("Serio", 1, 0, "EpisodeListModel", nullptr);
    engine.rootContext()->setContextProperty("tvShowViewModel", this);
    router.registerAction(serio::qt::ActionType::LOAD_TV_SHOW, [this] (const auto& args) { load(); });
    router.registerAction(serio::qt::ActionType::LOAD_EPISODES_LIST_PAGE, [this] (const auto& args) { loadEpisodes(args); });
    router.registerAction(serio::qt::ActionType::SHARE_CRAWLER_OF_CURRENT_TV_SHOW, [this] (const auto& args) { shareCrawler(); });
    router.registerAction(serio::qt::ActionType::CRAWL_CURRENT_TV_SHOW, [this] (const auto& args) { crawl(); });
    router.registerAction(serio::qt::ActionType::CONFIRM_CLEAR_CURRENT_TV_SHOW_WATCH_HISTORY, [this] (const auto& args) { confirmClearWatchHistory(); });
    router.registerAction(serio::qt::ActionType::CLEAR_CURRENT_TV_SHOW_WATCH_HISTORY, [this] (const auto& args) { clearWatchHistory(); });
    router.registerAction(serio::qt::ActionType::CONFIRM_DELETE_CURRENT_TV_SHOW, [this] (const auto& args) { confirmDeleteTvShow(); });
    router.registerAction(serio::qt::ActionType::DELETE_CURRENT_TV_SHOW, [this] (const auto& args) { deleteTvShow(); });
    router.registerAction(serio::qt::ActionType::OPEN_TV_SHOW_DETAILS_VIEW, [this] (const auto& args) { openTvShowDetails(); });
    connect(&episodeListModel, &serio::qt::EpisodeListModel::requestPageLoad,
            this, [&router] (auto offset, auto limit) { router.trigger(serio::qt::ActionType::LOAD_EPISODES_LIST_PAGE, QVariantList({offset, limit})); });
}

QString serio::qt::TvShowViewModel::getTvShowName() const {
    return tvShowName;
}

QString serio::qt::TvShowViewModel::getLastWatchDate() const {
    return lastWatchDate;
}

QString serio::qt::TvShowViewModel::getThumbnailUrl() const {
    return thumbnailUrl;
}

serio::qt::EpisodeListModel* serio::qt::TvShowViewModel::getEpisodeList() {
    return &episodeListModel;
}

void serio::qt::TvShowViewModel::load() {
    loadTvShow();
    modifyModel([this] { episodeListModel.requestFirstPageLoad(); });
}

void serio::qt::TvShowViewModel::loadEpisodes(const QVariantList& args) {
    auto episodes = viewer.getTvShowEpisodes(args[0].toUInt(),args[1].toUInt());
    modifyModel([this, episodes] { episodeListModel.loadPage(episodes); });
}

void serio::qt::TvShowViewModel::shareCrawler() {
    auto rawTvShowCrawler = viewer.getRawCrawlerOfSelectedTvShow();
    modifyModel([this, rawTvShowCrawler] {
        QGuiApplication::clipboard()->setText(QString::fromStdString(rawTvShowCrawler));
        snackbar.displayText("Crawler copied to your clipboard");
    });
}

void serio::qt::TvShowViewModel::loadTvShow() {
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

void serio::qt::TvShowViewModel::crawl() {
    try {
        stack.pushView(crawlingInProgressView);
        viewer.crawlSelectedTvShow();
        stack.popCurrentView();
    } catch (std::runtime_error& e) {
        stack.popCurrentView();
        throw e;
    }
}

void serio::qt::TvShowViewModel::confirmClearWatchHistory() {
    DialogModel model("Clear Watch History",
                      "You are about to clear your watch history of '" + tvShowName + "'.");
    model.setTopButtonAction(ActionType::CLEAR_CURRENT_TV_SHOW_WATCH_HISTORY);
    dialog.display(model);
}

void serio::qt::TvShowViewModel::clearWatchHistory() {
    viewer.clearSelectedTvShowWatchHistory();
    stack.popCurrentView();
    snackbar.displayText("Watch history cleared");
}

void serio::qt::TvShowViewModel::confirmDeleteTvShow() {
    DialogModel model("Delete TV Show",
                      "You are about to delete '" + tvShowName + "'");
    model.setTopButtonAction(serio::qt::ActionType::DELETE_CURRENT_TV_SHOW);
    dialog.display(model);
}

void serio::qt::TvShowViewModel::deleteTvShow() {
    viewer.deleteSelectedTvShow();
    stack.popAllViews();
}

void serio::qt::TvShowViewModel::openTvShowDetails() {
    stack.pushView(tvShowDetailsView);
}

QList<serio::qt::ButtonModel*> serio::qt::TvShowViewModel::getActions() const {
    return actions;
}
