package org.serio.core.applicationcontroller;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.model.CrawlerTypes;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.model.DaysAgoFormat;
import org.serio.core.applicationcontroller.tasks.*;
import org.serio.core.clipboard.Clipboard;
import org.serio.core.notifications.Notifications;
import org.serio.core.showplayer.ShowPlayer;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

public class ApplicationController {
    private final Clipboard clipboard;
    private final Notifications notifications;
    private final ShowPlayer showPlayer;
    private final Shows shows;
    private final ShowsCrawler showsCrawler;
    private final UserInterface userInterface;
    private final DateFormat lastWatchedDateFormat;
    private final EventStack eventStack;

    public ApplicationController(Clipboard clipboard, Notifications notifications, ShowPlayer showPlayer, Shows shows,
                                 ShowsCrawler showsCrawler, UserInterface userInterface) {
        this.clipboard = clipboard;
        this.notifications = notifications;
        this.showPlayer = showPlayer;
        this.shows = shows;
        this.showsCrawler = showsCrawler;
        this.userInterface = userInterface;
        lastWatchedDateFormat = new DaysAgoFormat();
        eventStack = new EventStack();
    }

    public synchronized void viewAllShows() {
        executeTask(new ViewAllShowsTask(shows, lastWatchedDateFormat));
    }

    public synchronized void addShow() {
        executeTask(new AddShowTask());
    }

    public synchronized void importShowFromJson() {
        executeTask(new ImportShowFromJsonTask());
    }

    public synchronized void selectShow(String showId) {
        executeTask(new SelectShowTask(showId, shows, lastWatchedDateFormat));
    }

    public synchronized void editThumbnailCrawler() {
        executeTask(new EditCrawlerTask(CrawlerTypes.THUMBNAIL));
    }

    public synchronized void editEpisodeVideoCrawler() {
        executeTask(new EditCrawlerTask(CrawlerTypes.EPISODE_VIDEO));
    }

    public synchronized void editEpisodeNameCrawler() {
        executeTask(new EditCrawlerTask(CrawlerTypes.EPISODE_NAME));
    }

    public void back() {

    }

    public void saveShowCrawler(String showCrawlerName) {

    }

    public void importShowCrawler(String rawShowCrawler) {

    }

    public void previewCrawler(String rawCrawler) {

    }

    public void saveCrawler(String rawCrawler) {

    }

    public void viewCrawlLog(String showId) {

    }

    public void viewLogEntry(int logEntryId) {

    }

    public void playShow() {

    }

    public void playShowEpisode(int episodeNumber) {

    }

    public void editShowCrawler() {

    }

    public void shareShowCrawler() {

    }

    public void crawlShow() {

    }

    public void viewShowCrawlLog() {

    }

    public void initiateClearShowWatchHistory() {

    }

    public void initiateShowRemoval() {

    }

    public void reportShowWatchProgress(double progress) {

    }

    public void playPreviousEpisode() {

    }

    public void playNextEpisode() {

    }

    public void clearWatchHistory() {

    }

    public void deleteShow() {

    }

    public void playShowFromTheBeginning() {

    }

    public void viewAllShows() {

    private void executeTask(ControllerTask task) {
        new HandleExceptionsTask(task).execute(eventStack, userInterface);
    }
}
