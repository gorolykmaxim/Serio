package org.serio.core.applicationcontroller;

import java.util.concurrent.ExecutorService;

/**
 * Proxy to an actual {@link ApplicationController}, that executes all of it's methods in the specified {@link ExecutorService}.
 */
public class BackgroundThreadApplicationControllerProxy implements ApplicationController {
    private final ApplicationController controller;
    private final ExecutorService service;

    /**
     * Construct a proxy.
     *
     * @param controller actual controller to delegate calls to
     * @param service executor service in which the specified controller's methods should be executed
     */
    public BackgroundThreadApplicationControllerProxy(ApplicationController controller, ExecutorService service) {
        this.controller = controller;
        this.service = service;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void viewAllShows() {
        service.execute(controller::viewAllShows);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void addShow() {
        service.execute(controller::addShow);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void importShowFromJson() {
        service.execute(controller::importShowFromJson);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setShowCrawler(String rawShowCrawler) {
        service.execute(() -> controller.setShowCrawler(rawShowCrawler));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void selectShow(String showId) {
        service.execute(() -> controller.selectShow(showId));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void editThumbnailCrawler() {
        service.execute(controller::editThumbnailCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void editEpisodeVideoCrawler() {
        service.execute(controller::editEpisodeVideoCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void editEpisodeNameCrawler() {
        service.execute(controller::editEpisodeNameCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setCrawler(String rawCrawler) {
        service.execute(() -> controller.setCrawler(rawCrawler));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void saveCrawler() {
        service.execute(controller::saveCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setShowName(String showName) {
        service.execute(() -> controller.setShowName(showName));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void saveShowCrawler() {
        service.execute(controller::saveShowCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void importShowCrawler() {
        service.execute(controller::importShowCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void previewCrawler() {
        service.execute(controller::previewCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void viewCrawlLog() {
        service.execute(controller::viewCrawlLog);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void viewLogEntry(int logEntryId) {
        service.execute(() -> controller.viewLogEntry(logEntryId));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void playShow() {
        service.execute(controller::playShow);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void playShowEpisode(int episodeNumber) {
        service.execute(() -> controller.playShowEpisode(episodeNumber));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void editShowCrawler() {
        service.execute(controller::editShowCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void shareShowCrawler() {
        service.execute(controller::shareShowCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void crawlShow() {
        service.execute(controller::crawlShow);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void viewShowCrawlLog() {
        service.execute(controller::viewShowCrawlLog);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void initiateClearShowWatchHistory() {
        service.execute(controller::initiateClearShowWatchHistory);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void initiateShowRemoval() {
        service.execute(controller::initiateShowRemoval);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void reportShowWatchProgress(double progress) {
        service.execute(() -> controller.reportShowWatchProgress(progress));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void playPreviousEpisode() {
        service.execute(controller::playPreviousEpisode);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void playNextEpisode() {
        service.execute(controller::playNextEpisode);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void clearWatchHistory() {
        service.execute(controller::clearWatchHistory);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void deleteShow() {
        service.execute(controller::deleteShow);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void playShowFromTheBeginning() {
        service.execute(controller::playShowFromTheBeginning);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void confirmShowOverride() {
        service.execute(controller::confirmShowOverride);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void displayError(String error) {
        service.execute(() -> controller.displayError(error));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void back() {
        service.execute(controller::back);
    }
}
