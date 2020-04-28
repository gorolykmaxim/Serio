package org.serio.core.applicationcontroller;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.model.CrawlerTypes;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.model.DaysAgoFormat;
import org.serio.core.applicationcontroller.tasks.BackTask;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.applicationcontroller.tasks.HandleExceptionsTask;
import org.serio.core.applicationcontroller.tasks.ViewAllShowsTask;
import org.serio.core.applicationcontroller.tasks.allshows.AddShowTask;
import org.serio.core.applicationcontroller.tasks.allshows.ImportShowFromJsonTask;
import org.serio.core.applicationcontroller.tasks.allshows.SelectShowTask;
import org.serio.core.applicationcontroller.tasks.crawllog.ViewLogEntryTask;
import org.serio.core.applicationcontroller.tasks.crawlpreview.ViewCrawlLogTask;
import org.serio.core.applicationcontroller.tasks.editcrawler.PreviewCrawlerTask;
import org.serio.core.applicationcontroller.tasks.editcrawler.SaveCrawlerTask;
import org.serio.core.applicationcontroller.tasks.editshowcrawler.EditCrawlerTask;
import org.serio.core.applicationcontroller.tasks.editshowcrawler.SaveShowCrawlerTask;
import org.serio.core.applicationcontroller.tasks.importshow.ImportShowCrawlerTask;
import org.serio.core.applicationcontroller.tasks.showdetails.*;
import org.serio.core.applicationcontroller.tasks.showplayer.PlayNextEpisodeTask;
import org.serio.core.applicationcontroller.tasks.showplayer.PlayPreviousEpisodeTask;
import org.serio.core.applicationcontroller.tasks.showplayer.PlayShowFromTheBeginningTask;
import org.serio.core.applicationcontroller.tasks.showplayer.ReportShowWatchProgressTask;
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

    public synchronized void saveCrawler(String rawCrawler) {
        executeTask(new SaveCrawlerTask(rawCrawler));
    }

    public synchronized void saveShowCrawler(String showCrawlerName) {
        executeTask(new SaveShowCrawlerTask(showCrawlerName, shows, showsCrawler, lastWatchedDateFormat));
    }

    public synchronized void importShowCrawler(String rawShowCrawler) {
        executeTask(new ImportShowCrawlerTask(rawShowCrawler, shows, showsCrawler, lastWatchedDateFormat));
    }

    public synchronized void previewCrawler(String rawCrawler) {
        executeTask(new PreviewCrawlerTask(rawCrawler, showsCrawler));
    }

    public synchronized void viewCrawlLog() {
        executeTask(new ViewCrawlLogTask());
    }

    public synchronized void viewLogEntry(int logEntryId) {
        executeTask(new ViewLogEntryTask(logEntryId));
    }

    public synchronized void playShow() {
        executeTask(new PlayShowTask(showPlayer));
    }

    public synchronized void playShowEpisode(int episodeNumber) {
        executeTask(new PlayShowEpisodeTask(episodeNumber, showPlayer));
    }

    public synchronized void editShowCrawler() {
        executeTask(new EditShowCrawlerTask(showsCrawler));
    }

    public synchronized void shareShowCrawler() {
        executeTask(new ShareShowCrawlerTask(notifications, clipboard, showsCrawler));
    }

    public synchronized void crawlShow() {
        executeTask(new CrawlShowTask(showsCrawler, shows, lastWatchedDateFormat));
    }

    public synchronized void viewShowCrawlLog() {
        executeTask(new ViewShowCrawlLogTask(showsCrawler));
    }

    public synchronized void initiateClearShowWatchHistory() {
        executeTask(new InitiateClearShowWatchHistoryTask());
    }

    public synchronized void initiateShowRemoval() {
        executeTask(new InitiateShowRemovalTask());
    }

    public synchronized void reportShowWatchProgress(double progress) {
        executeTask(new ReportShowWatchProgressTask(progress, showPlayer));
    }

    public synchronized void playPreviousEpisode() {
        executeTask(new PlayPreviousEpisodeTask(showPlayer));
    }

    public synchronized void playNextEpisode() {
        executeTask(new PlayNextEpisodeTask(showPlayer));
    }

    public synchronized void clearWatchHistory() {
        executeTask(new ClearWatchHistoryTask(shows, lastWatchedDateFormat));
    }

    public synchronized void deleteShow() {
        executeTask(new DeleteShowTask(shows, showsCrawler, lastWatchedDateFormat));
    }

    public synchronized void playShowFromTheBeginning() {
        executeTask(new PlayShowFromTheBeginningTask(showPlayer));
    }

    public synchronized void back() {
        executeTask(new BackTask());
    }

    private void executeTask(ControllerTask task) {
        new HandleExceptionsTask(task).execute(eventStack, userInterface);
    }
}
