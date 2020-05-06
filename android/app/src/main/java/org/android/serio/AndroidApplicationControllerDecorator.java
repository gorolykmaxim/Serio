package org.android.serio;

import android.webkit.JavascriptInterface;

import org.serio.core.applicationcontroller.ApplicationController;

public class AndroidApplicationControllerDecorator implements ApplicationController {
    private final ApplicationController controller;

    public AndroidApplicationControllerDecorator(ApplicationController controller) {
        this.controller = controller;
    }

    @JavascriptInterface
    @Override
    public void viewAllShows() {
        controller.viewAllShows();
    }

    @JavascriptInterface
    @Override
    public void addShow() {
        controller.addShow();
    }

    @JavascriptInterface
    @Override
    public void importShowFromJson() {
        controller.importShowFromJson();
    }

    @JavascriptInterface
    @Override
    public void setShowCrawler(String rawShowCrawler) {
        controller.setShowCrawler(rawShowCrawler);
    }

    @JavascriptInterface
    @Override
    public void selectShow(String showId) {
        controller.selectShow(showId);
    }

    @JavascriptInterface
    @Override
    public void editThumbnailCrawler() {
        controller.editThumbnailCrawler();
    }

    @JavascriptInterface
    @Override
    public void editEpisodeVideoCrawler() {
        controller.editEpisodeVideoCrawler();
    }

    @JavascriptInterface
    @Override
    public void editEpisodeNameCrawler() {
        controller.editEpisodeNameCrawler();
    }

    @JavascriptInterface
    @Override
    public void setCrawler(String rawCrawler) {
        controller.setCrawler(rawCrawler);
    }

    @JavascriptInterface
    @Override
    public void saveCrawler() {
        controller.saveCrawler();
    }

    @JavascriptInterface
    @Override
    public void setShowName(String showName) {
        controller.setShowName(showName);
    }

    @JavascriptInterface
    @Override
    public void saveShowCrawler() {
        controller.saveShowCrawler();
    }

    @JavascriptInterface
    @Override
    public void importShowCrawler() {
        controller.importShowCrawler();
    }

    @JavascriptInterface
    @Override
    public void previewCrawler() {
        controller.previewCrawler();
    }

    @JavascriptInterface
    @Override
    public void viewCrawlLog() {
        controller.viewCrawlLog();
    }

    @JavascriptInterface
    @Override
    public void viewLogEntry(int logEntryId) {
        controller.viewLogEntry(logEntryId);
    }

    @JavascriptInterface
    @Override
    public void playShow() {
        controller.playShow();
    }

    @JavascriptInterface
    @Override
    public void playShowEpisode(int episodeNumber) {
        controller.playShowEpisode(episodeNumber);
    }

    @JavascriptInterface
    @Override
    public void editShowCrawler() {
        controller.editShowCrawler();
    }

    @JavascriptInterface
    @Override
    public void shareShowCrawler() {
        controller.shareShowCrawler();
    }

    @JavascriptInterface
    @Override
    public void crawlShow() {
        controller.crawlShow();
    }

    @JavascriptInterface
    @Override
    public void viewShowCrawlLog() {
        controller.viewShowCrawlLog();
    }

    @JavascriptInterface
    @Override
    public void initiateClearShowWatchHistory() {
        controller.initiateClearShowWatchHistory();
    }

    @JavascriptInterface
    @Override
    public void initiateShowRemoval() {
        controller.initiateShowRemoval();
    }

    @JavascriptInterface
    @Override
    public void reportShowWatchProgress(double progress) {
        controller.reportShowWatchProgress(progress);
    }

    @JavascriptInterface
    @Override
    public void playPreviousEpisode() {
        controller.playPreviousEpisode();
    }

    @JavascriptInterface
    @Override
    public void playNextEpisode() {
        controller.playNextEpisode();
    }

    @JavascriptInterface
    @Override
    public void clearWatchHistory() {
        controller.clearWatchHistory();
    }

    @JavascriptInterface
    @Override
    public void deleteShow() {
        controller.deleteShow();
    }

    @JavascriptInterface
    @Override
    public void playShowFromTheBeginning() {
        controller.playShowFromTheBeginning();
    }

    @JavascriptInterface
    @Override
    public void confirmShowOverride() {
        controller.confirmShowOverride();
    }

    @JavascriptInterface
    @Override
    public void displayError(String error) {
        controller.displayError(error);
    }

    @JavascriptInterface
    @Override
    public void back() {
        controller.back();
    }
}
