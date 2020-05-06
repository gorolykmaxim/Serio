package org.android.serio;

import android.webkit.JavascriptInterface;

import org.serio.core.applicationcontroller.ApplicationController;

/**
 * Decorator of {@link ApplicationController}, that decorates each of it's methods with
 * {@link JavascriptInterface}, so that they can be called from the {@link android.webkit.WebView}.
 */
public class AndroidApplicationControllerDecorator implements ApplicationController {
    private final ApplicationController controller;

    /**
     * Construct a controller decorator.
     * 
     * @param controller controller to decorate
     */
    public AndroidApplicationControllerDecorator(ApplicationController controller) {
        this.controller = controller;
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void viewAllShows() {
        controller.viewAllShows();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void addShow() {
        controller.addShow();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void importShowFromJson() {
        controller.importShowFromJson();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void setShowCrawler(String rawShowCrawler) {
        controller.setShowCrawler(rawShowCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void selectShow(String showId) {
        controller.selectShow(showId);
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void editThumbnailCrawler() {
        controller.editThumbnailCrawler();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void editEpisodeVideoCrawler() {
        controller.editEpisodeVideoCrawler();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void editEpisodeNameCrawler() {
        controller.editEpisodeNameCrawler();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void setCrawler(String rawCrawler) {
        controller.setCrawler(rawCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void saveCrawler() {
        controller.saveCrawler();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void setShowName(String showName) {
        controller.setShowName(showName);
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void saveShowCrawler() {
        controller.saveShowCrawler();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void importShowCrawler() {
        controller.importShowCrawler();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void previewCrawler() {
        controller.previewCrawler();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void viewCrawlLog() {
        controller.viewCrawlLog();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void viewLogEntry(int logEntryId) {
        controller.viewLogEntry(logEntryId);
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void playShow() {
        controller.playShow();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void playShowEpisode(int episodeNumber) {
        controller.playShowEpisode(episodeNumber);
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void editShowCrawler() {
        controller.editShowCrawler();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void shareShowCrawler() {
        controller.shareShowCrawler();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void crawlShow() {
        controller.crawlShow();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void viewShowCrawlLog() {
        controller.viewShowCrawlLog();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void initiateClearShowWatchHistory() {
        controller.initiateClearShowWatchHistory();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void initiateShowRemoval() {
        controller.initiateShowRemoval();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void reportShowWatchProgress(double progress) {
        controller.reportShowWatchProgress(progress);
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void playPreviousEpisode() {
        controller.playPreviousEpisode();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void playNextEpisode() {
        controller.playNextEpisode();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void clearWatchHistory() {
        controller.clearWatchHistory();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void deleteShow() {
        controller.deleteShow();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void playShowFromTheBeginning() {
        controller.playShowFromTheBeginning();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void confirmShowOverride() {
        controller.confirmShowOverride();
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void displayError(String error) {
        controller.displayError(error);
    }

    /**
     * {@inheritDoc}
     */
    @JavascriptInterface
    @Override
    public void back() {
        controller.back();
    }
}
