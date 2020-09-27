#include "TvShowViewModel.h"
#include <QQmlContext>
#include <QGuiApplication>
#include <QClipboard>

serio::qt::TvShowViewModel::TvShowViewModel(unsigned int pageSize, unsigned int pageCountLimit,
                                            serio::core::TvShowViewer &viewer, serio::qt::SnackbarViewModel& snackbar)
    : episodeListModel(pageSize, pageCountLimit), viewer(viewer), snackbar(snackbar) {}

void serio::qt::TvShowViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine &engine) {
    qmlRegisterUncreatableType<EpisodeListModel>("Serio", 1, 0, "EpisodeListModel", nullptr);
    engine.rootContext()->setContextProperty("tvShowViewModel", this);
    router.registerAction(serio::qt::ActionType::LOAD_TV_SHOW, [this] (const QVariantList& args) { load(); });
    router.registerAction(serio::qt::ActionType::LOAD_EPISODES_LIST_PAGE, [this] (const QVariantList& args) { loadEpisodes(args); });
    router.registerAction(serio::qt::ActionType::SHARE_CRAWLER_OF_CURRENT_TV_SHOW, [this] (const QVariantList& args) { shareCrawler(); });
    connect(&episodeListModel, &serio::qt::EpisodeListModel::requestPageLoad,
            this, [&router] (unsigned int offset, unsigned int limit) { router.trigger(serio::qt::ActionType::LOAD_EPISODES_LIST_PAGE, QVariantList({offset, limit})); });
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
    serio::core::ListPage<serio::core::Episode> episodes = viewer.getTvShowEpisodes(args[0].toUInt(),args[1].toUInt());
    modifyModel([this, episodes] { episodeListModel.loadPage(episodes); });
}

void serio::qt::TvShowViewModel::shareCrawler() {
    std::string rawTvShowCrawler = viewer.getRawCrawlerOfSelectedTvShow();
    QGuiApplication::clipboard()->setText(QString::fromStdString(rawTvShowCrawler));
    snackbar.displayText("Crawler copied to your clipboard");
}

void serio::qt::TvShowViewModel::loadTvShow() {
    serio::core::TvShow tvShow = viewer.getSelectedTvShow();
    modifyModel([this, tvShow] {
        tvShowName = QString::fromStdString(tvShow.getName());
        thumbnailUrl = QString::fromStdString(tvShow.getThumbnailUrl());
        std::optional<serio::core::LastWatchDate> date = tvShow.getLastWatchDate();
        lastWatchDate = QString::fromStdString(date ? "Last watched " + date->toString() : "");
        emit selectedTvShowChanged();
    });
}
