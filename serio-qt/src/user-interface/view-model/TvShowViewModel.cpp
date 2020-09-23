#include "TvShowViewModel.h"
#include <QQmlContext>

serio::qt::TvShowViewModel::TvShowViewModel(unsigned int pageSize, unsigned int pageCountLimit,
                                            serio::core::TvShowStorage &storage, serio::qt::StackOfViews &stack)
    : pageSize(pageSize), episodeListModel(pageSize, pageCountLimit), storage(storage), stack(stack) {}

void serio::qt::TvShowViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine &engine) {
    qmlRegisterUncreatableType<EpisodeListModel>("Serio", 1, 0, "EpisodeListModel", nullptr);
    engine.rootContext()->setContextProperty("tvShowViewModel", this);
    router.registerAction(serio::qt::ActionType::OPEN_TV_SHOW_VIEW, [this] (const QVariantList& args) { openView(args); });
    router.registerAction(serio::qt::ActionType::LOAD_TV_SHOW, [this] (const QVariantList& args) { load(); });
    router.registerAction(serio::qt::ActionType::LOAD_EPISODES_LIST_PAGE, [this] (const QVariantList& args) { loadEpisodes(args); });
}

QString serio::qt::TvShowViewModel::getTvShowName() const {
    return selectedTvShow ? QString::fromStdString(selectedTvShow->getName()) : QString();
}

QString serio::qt::TvShowViewModel::getLastWatchDate() const {
    if (selectedTvShow && selectedTvShow->getLastWatchDate()) {
        return QString::fromStdString("Last watched " + selectedTvShow->getLastWatchDate()->toString());
    } else {
        return QString();
    }
}

QString serio::qt::TvShowViewModel::getThumbnailUrl() const {
    return selectedTvShow ? QString::fromStdString(selectedTvShow->getThumbnailUrl()) : QString();
}

serio::qt::EpisodeListModel* serio::qt::TvShowViewModel::getEpisodeList() {
    return &episodeListModel;
}

void serio::qt::TvShowViewModel::openView(const QList<QVariant> &args) {
    selectedTvShowName = args[0].toString();
    stack.pushView("TvShowView.qml");
}

void serio::qt::TvShowViewModel::load() {
    loadTvShow();
    loadEpisodes(QVariantList({0, pageSize}));
}

void serio::qt::TvShowViewModel::loadEpisodes(const QVariantList& args) {
    serio::core::ListPage<serio::core::Episode> episodes = storage.getEpisodesOfTvShowWithName(
            selectedTvShow->getName(),
            args[0].toUInt(),
            args[1].toUInt());
    modifyModel([this, episodes] { episodeListModel.loadPage(episodes); });
}

void serio::qt::TvShowViewModel::loadTvShow() {
    selectedTvShow = storage.getTvShowByName(selectedTvShowName->toStdString());
    emit selectedTvShowChanged();
}
