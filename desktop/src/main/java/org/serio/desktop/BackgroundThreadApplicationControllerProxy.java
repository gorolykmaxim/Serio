package org.serio.desktop;

import org.serio.core.applicationcontroller.ApplicationController;

import java.util.concurrent.ExecutorService;

/**
 * Proxy to {@link ApplicationController}, that executes all of the latter ones' methods on a background thread
 * in order to not block the UI thread.
 */
public class BackgroundThreadApplicationControllerProxy {
    private final ApplicationController controller;
    private final ExecutorService service;

    /**
     * Construct a proxy.
     *
     * @param controller controller to decorate
     * @param service executor service on which all of the controller's methods should be executed
     */
    public BackgroundThreadApplicationControllerProxy(ApplicationController controller, ExecutorService service) {
        this.controller = controller;
        this.service = service;
    }

    /**
     * @see ApplicationController#viewAllShows()
     */
    public void viewAllShows() {
        service.execute(controller::viewAllShows);
    }

    /**
     * @see ApplicationController#addShow()
     */
    public void addShow() {
        service.execute(controller::addShow);
    }

    /**
     * @see ApplicationController#importShowFromJson()
     */
    public void importShowFromJson() {
        service.execute(controller::importShowFromJson);
    }

    /**
     * @see ApplicationController#setShowCrawler(String)
     */
    public void setShowCrawler(String rawShowCrawler) {
        service.execute(() -> controller.setShowCrawler(rawShowCrawler));
    }

    /**
     * @see ApplicationController#selectShow(String)
     */
    public void selectShow(String showId) {
        service.execute(() -> controller.selectShow(showId));
    }

    /**
     * @see ApplicationController#editThumbnailCrawler()
     */
    public void editThumbnailCrawler() {
        service.execute(controller::editThumbnailCrawler);
    }

    /**
     * @see ApplicationController#editEpisodeVideoCrawler()
     */
    public void editEpisodeVideoCrawler() {
        service.execute(controller::editEpisodeVideoCrawler);
    }

    /**
     * @see ApplicationController#editEpisodeNameCrawler()
     */
    public void editEpisodeNameCrawler() {
        service.execute(controller::editEpisodeNameCrawler);
    }

    /**
     * @see ApplicationController#setCrawler(String)
     */
    public void setCrawler(String rawCrawler) {
        service.execute(() -> controller.setCrawler(rawCrawler));
    }

    /**
     * @see ApplicationController#saveCrawler()
     */
    public void saveCrawler() {
        service.execute(controller::saveCrawler);
    }

    /**
     * @see ApplicationController#setShowName(String)
     */
    public void setShowName(String showName) {
        service.execute(() -> controller.setShowName(showName));
    }

    /**
     * @see ApplicationController#saveShowCrawler()
     */
    public void saveShowCrawler() {
        service.execute(controller::saveShowCrawler);
    }

    /**
     * @see ApplicationController#importShowCrawler()
     */
    public void importShowCrawler() {
        service.execute(controller::importShowCrawler);
    }

    /**
     * @see ApplicationController#previewCrawler()
     */
    public void previewCrawler() {
        service.execute(controller::previewCrawler);
    }

    /**
     * @see ApplicationController#viewCrawlLog()
     */
    public void viewCrawlLog() {
        service.execute(controller::viewCrawlLog);
    }

    /**
     * @see ApplicationController#viewLogEntry(int)
     */
    public void viewLogEntry(int logEntryId) {
        service.execute(() -> controller.viewLogEntry(logEntryId));
    }

    /**
     * @see ApplicationController#playShow()
     */
    public void playShow() {
        service.execute(controller::playShow);
    }

    /**
     * @see ApplicationController#playShowEpisode(int)
     */
    public void playShowEpisode(int episodeNumber) {
        service.execute(() -> controller.playShowEpisode(episodeNumber));
    }

    /**
     * @see ApplicationController#editShowCrawler()
     */
    public void editShowCrawler() {
        service.execute(controller::editShowCrawler);
    }

    /**
     * @see ApplicationController#shareShowCrawler()
     */
    public void shareShowCrawler() {
        service.execute(controller::shareShowCrawler);
    }

    /**
     * @see ApplicationController#crawlShow()
     */
    public void crawlShow() {
        service.execute(controller::crawlShow);
    }

    /**
     * @see ApplicationController#viewShowCrawlLog()
     */
    public void viewShowCrawlLog() {
        service.execute(controller::viewShowCrawlLog);
    }

    /**
     * @see ApplicationController#initiateClearShowWatchHistory()
     */
    public void initiateClearShowWatchHistory() {
        service.execute(controller::initiateClearShowWatchHistory);
    }

    /**
     * @see ApplicationController#initiateShowRemoval()
     */
    public void initiateShowRemoval() {
        service.execute(controller::initiateShowRemoval);
    }

    /**
     * @see ApplicationController#reportShowWatchProgress(double)
     */
    public void reportShowWatchProgress(double progress) {
        service.execute(() -> controller.reportShowWatchProgress(progress));
    }

    /**
     * @see ApplicationController#playPreviousEpisode()
     */
    public void playPreviousEpisode() {
        service.execute(controller::playPreviousEpisode);
    }

    /**
     * @see ApplicationController#playNextEpisode()
     */
    public void playNextEpisode() {
        service.execute(controller::playNextEpisode);
    }

    /**
     * @see ApplicationController#clearWatchHistory()
     */
    public void clearWatchHistory() {
        service.execute(controller::clearWatchHistory);
    }

    /**
     * @see ApplicationController#deleteShow()
     */
    public void deleteShow() {
        service.execute(controller::deleteShow);
    }

    /**
     * @see ApplicationController#playShowFromTheBeginning()
     */
    public void playShowFromTheBeginning() {
        service.execute(controller::playShowFromTheBeginning);
    }

    /**
     * @see ApplicationController#confirmShowOverride()
     */
    public void confirmShowOverride() {
        service.execute(controller::confirmShowOverride);
    }

    /**
     * @see ApplicationController#displayError(String)
     */
    public void displayError(String error) {
        service.execute(() -> controller.displayError(error));
    }

    /**
     * @see ApplicationController#back()
     */
    public void back() {
        service.execute(controller::back);
    }
}
