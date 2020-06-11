package org.android.serio;

import android.webkit.JavascriptInterface;

import org.serio.core.applicationcontroller.ApplicationController;

/**
 * Decorator of {@link ApplicationController}, that decorates each of it's methods with
 * {@link JavascriptInterface}, so that they can be called from the {@link android.webkit.WebView}.
 */
public class AndroidApplicationControllerDecorator {
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
     * @see ApplicationController#viewAllShows()
     */
    @JavascriptInterface
    public void viewAllShows() {
        controller.viewAllShows();
    }

    /**
     * @see ApplicationController#addShow()
     */
    @JavascriptInterface
    public void addShow() {
        controller.addShow();
    }

    /**
     * @see ApplicationController#importShowFromJson()
     */
    @JavascriptInterface
    public void importShowFromJson() {
        controller.importShowFromJson();
    }

    /**
     * @see ApplicationController#setShowCrawler(String)
     */
    @JavascriptInterface
    public void setShowCrawler(String rawShowCrawler) {
        controller.setShowCrawler(rawShowCrawler);
    }

    /**
     * @see ApplicationController#selectShow(String)
     */
    @JavascriptInterface
    public void selectShow(String showId) {
        controller.selectShow(showId);
    }

    /**
     * @see ApplicationController#editThumbnailCrawler()
     */
    @JavascriptInterface
    public void editThumbnailCrawler() {
        controller.editThumbnailCrawler();
    }

    /**
     * @see ApplicationController#editEpisodeVideoCrawler()
     */
    @JavascriptInterface
    public void editEpisodeVideoCrawler() {
        controller.editEpisodeVideoCrawler();
    }

    /**
     * @see ApplicationController#editEpisodeNameCrawler()
     */
    @JavascriptInterface
    public void editEpisodeNameCrawler() {
        controller.editEpisodeNameCrawler();
    }

    /**
     * @see ApplicationController#setCrawler(String)
     */
    @JavascriptInterface
    public void setCrawler(String rawCrawler) {
        controller.setCrawler(rawCrawler);
    }

    /**
     * @see ApplicationController#saveCrawler()
     */
    @JavascriptInterface
    public void saveCrawler() {
        controller.saveCrawler();
    }

    /**
     * @see ApplicationController#setShowName(String)
     */
    @JavascriptInterface
    public void setShowName(String showName) {
        controller.setShowName(showName);
    }

    /**
     * @see ApplicationController#saveShowCrawler()
     */
    @JavascriptInterface
    public void saveShowCrawler() {
        controller.saveShowCrawler();
    }

    /**
     * @see ApplicationController#importShowCrawler()
     */
    @JavascriptInterface
    public void importShowCrawler() {
        controller.importShowCrawler();
    }

    /**
     * @see ApplicationController#previewCrawler()
     */
    @JavascriptInterface
    public void previewCrawler() {
        controller.previewCrawler();
    }

    /**
     * @see ApplicationController#viewCrawlLog()
     */
    @JavascriptInterface
    public void viewCrawlLog() {
        controller.viewCrawlLog();
    }

    /**
     * @see ApplicationController#viewLogEntry(int)
     */
    @JavascriptInterface
    public void viewLogEntry(int logEntryId) {
        controller.viewLogEntry(logEntryId);
    }

    /**
     * @see ApplicationController#playShow()
     */
    @JavascriptInterface
    public void playShow() {
        controller.playShow();
    }

    /**
     * @see ApplicationController#playShowEpisode(int)
     */
    @JavascriptInterface
    public void playShowEpisode(int episodeNumber) {
        controller.playShowEpisode(episodeNumber);
    }

    /**
     * @see ApplicationController#editShowCrawler()
     */
    @JavascriptInterface
    public void editShowCrawler() {
        controller.editShowCrawler();
    }

    /**
     * @see ApplicationController#shareShowCrawler()
     */
    @JavascriptInterface
    public void shareShowCrawler() {
        controller.shareShowCrawler();
    }

    /**
     * @see ApplicationController#crawlShow()
     */
    @JavascriptInterface
    public void crawlShow() {
        controller.crawlShow();
    }

    /**
     * @see ApplicationController#viewShowCrawlLog()
     */
    @JavascriptInterface
    public void viewShowCrawlLog() {
        controller.viewShowCrawlLog();
    }

    /**
     * @see ApplicationController#initiateClearShowWatchHistory()
     */
    @JavascriptInterface
    public void initiateClearShowWatchHistory() {
        controller.initiateClearShowWatchHistory();
    }

    /**
     * @see ApplicationController#initiateShowRemoval()
     */
    @JavascriptInterface
    public void initiateShowRemoval() {
        controller.initiateShowRemoval();
    }

    /**
     * @see ApplicationController#reportShowWatchProgress(double)
     */
    @JavascriptInterface
    public void reportShowWatchProgress(double progress) {
        controller.reportShowWatchProgress(progress);
    }

    /**
     * @see ApplicationController#playPreviousEpisode()
     */
    @JavascriptInterface
    public void playPreviousEpisode() {
        controller.playPreviousEpisode();
    }

    /**
     * @see ApplicationController#playNextEpisode()
     */
    @JavascriptInterface
    public void playNextEpisode() {
        controller.playNextEpisode();
    }

    /**
     * @see ApplicationController#clearWatchHistory()
     */
    @JavascriptInterface
    public void clearWatchHistory() {
        controller.clearWatchHistory();
    }

    /**
     * @see ApplicationController#deleteShow()
     */
    @JavascriptInterface
    public void deleteShow() {
        controller.deleteShow();
    }

    /**
     * @see ApplicationController#playShowFromTheBeginning()
     */
    @JavascriptInterface
    public void playShowFromTheBeginning() {
        controller.playShowFromTheBeginning();
    }

    /**
     * @see ApplicationController#confirmShowOverride()
     */
    @JavascriptInterface
    public void confirmShowOverride() {
        controller.confirmShowOverride();
    }

    /**
     * @see ApplicationController#displayError(String)
     */
    @JavascriptInterface
    public void displayError(String error) {
        controller.displayError(error);
    }

    /**
     * @see ApplicationController#displayCrawlerHelp()
     */
    @JavascriptInterface
    public void displayCrawlerHelp() {
        controller.displayCrawlerHelp();
    }

    /**
     * @see ApplicationController#back()
     */
    @JavascriptInterface
    public void back() {
        controller.back();
    }
}
