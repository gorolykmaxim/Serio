package org.serio.desktop;

import org.serio.core.applicationcontroller.ApplicationController;

import java.util.concurrent.ExecutorService;

public class BackgroundThreadApplicationControllerProxy {
    private final ApplicationController controller;
    private final ExecutorService service;

    public BackgroundThreadApplicationControllerProxy(ApplicationController controller, ExecutorService service) {
        this.controller = controller;
        this.service = service;
    }

    public void viewAllShows() {
        service.execute(controller::viewAllShows);
    }
    
    public void addShow() {
        service.execute(controller::addShow);
    }
    
    public void importShowFromJson() {
        service.execute(controller::importShowFromJson);
    }
    
    public void setShowCrawler(String rawShowCrawler) {
        service.execute(() -> controller.setShowCrawler(rawShowCrawler));
    }
    
    public void selectShow(String showId) {
        service.execute(() -> controller.selectShow(showId));
    }
    
    public void editThumbnailService() {
        service.execute(controller::editThumbnailCrawler);
    }

    public void editEpisodeVideoCrawler() {
        service.execute(controller::editEpisodeVideoCrawler);
    }

    public void editEpisodeNameCrawler() {
        service.execute(controller::editEpisodeNameCrawler);
    }

    public void setCrawler(String rawCrawler) {
        service.execute(() -> controller.setCrawler(rawCrawler));
    }

    public void saveCrawler() {
        service.execute(controller::saveCrawler);
    }

    public void setShowName(String showName) {
        service.execute(() -> controller.setShowName(showName));
    }

    public void saveShowCrawler() {
        service.execute(controller::saveShowCrawler);
    }

    public void importShowCrawler() {
        service.execute(controller::importShowCrawler);
    }

    public void previewCrawler() {
        service.execute(controller::previewCrawler);
    }

    public void viewCrawlLog() {
        service.execute(controller::viewCrawlLog);
    }

    public void viewLogEntry(int logEntryId) {
        service.execute(() -> controller.viewLogEntry(logEntryId));
    }

    public void playShow() {
        service.execute(controller::playShow);
    }

    public void playShowEpisode(int episodeNumber) {
        service.execute(() -> controller.playShowEpisode(episodeNumber));
    }

    public void editShowCrawler() {
        service.execute(controller::editShowCrawler);
    }

    public void shareShowCrawler() {
        service.execute(controller::shareShowCrawler);
    }

    public void crawlShow() {
        service.execute(controller::crawlShow);
    }

    public void viewShowCrawlLog() {
        service.execute(controller::viewShowCrawlLog);
    }

    public void initiateClearShowWatchHistory() {
        service.execute(controller::initiateClearShowWatchHistory);
    }

    public void initiateShowRemoval() {
        service.execute(controller::initiateShowRemoval);
    }

    public void reportShowWatchProgress(double progress) {
        service.execute(() -> controller.reportShowWatchProgress(progress));
    }

    public void playPreviousEpisode() {
        service.execute(controller::playPreviousEpisode);
    }

    public void playNextEpisode() {
        service.execute(controller::playNextEpisode);
    }

    public void clearWatchHistory() {
        service.execute(controller::clearWatchHistory);
    }

    public void deleteShow() {
        service.execute(controller::deleteShow);
    }

    public void playShowFromTheBeginning() {
        service.execute(controller::playShowFromTheBeginning);
    }

    public void confirmShowOverride() {
        service.execute(controller::confirmShowOverride);
    }

    public void displayError(String error) {
        service.execute(() -> controller.displayError(error));
    }

    public void back() {
        service.execute(controller::back);
    }
}
