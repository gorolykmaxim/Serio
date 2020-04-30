package org.serio.core.applicationcontroller;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.model.CrawlerTypes;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.model.DaysAgoFormat;
import org.serio.core.applicationcontroller.tasks.*;
import org.serio.core.applicationcontroller.tasks.allshows.AddShowTask;
import org.serio.core.applicationcontroller.tasks.allshows.ImportShowFromJsonTask;
import org.serio.core.applicationcontroller.tasks.allshows.SelectShowTask;
import org.serio.core.applicationcontroller.tasks.crawllog.ViewLogEntryTask;
import org.serio.core.applicationcontroller.tasks.crawlpreview.ViewCrawlLogTask;
import org.serio.core.applicationcontroller.tasks.editcrawler.PreviewCrawlerTask;
import org.serio.core.applicationcontroller.tasks.editcrawler.SaveCrawlerTask;
import org.serio.core.applicationcontroller.tasks.editshowcrawler.EditCrawlerTask;
import org.serio.core.applicationcontroller.tasks.editshowcrawler.SaveShowCrawlerWithoutOverrideTask;
import org.serio.core.applicationcontroller.tasks.importshow.ImportShowCrawlerWithoutOverrideTask;
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

/**
 * Controller of the application's view state.
 *
 * <p>You can consider it as a controller in terms of the model-view-controller architecture, since it does
 * basically the same thing: reacts to user input, call corresponding methods in other modules and updates
 * currently displayed view if necessary.</p>
 *
 * <p>Controller interacts with {@link UserInterface} by sending one-way {@link org.serio.core.userinterface.ApplicationEvent}
 * to it. Upon receiving an event, the first one renders a corresponding view, thus the {@link ApplicationController}
 * is the one who decides what is getting rendered and when.</p>
 *
 * <p>{@link ApplicationController} keeps track of all the events sent to the {@link UserInterface}. Those events
 * are kept in a corresponding stack, which represents the current tree of opened views: the view that is
 * currently displayed to the user is on top of the stack and the view, that was displayed previously and can
 * be switched back to is right below it in the stack. Each view in the stack is represented by the corresponding
 * {@link org.serio.core.userinterface.ApplicationEvent}, that was sent to the {@link UserInterface} to render
 * that view.</p>
 *
 * <p>{@link ApplicationController} keeps all the necessary context information in it's events. Thus, the controller
 * tries as hard as it can to not query the information it has already queried in the past. If the information,
 * that should be rendered, can be obtained from one of the events in the stack - the controller will traverse
 * the stack, find that information and re-use it. The controller will query information from the external module
 * only in case when such information, present in the events, is out of date.</p>
 *
 * <p>The controller supports the "Back" action: when called it will pop the latest event from the stack
 * and send the previous event to the {@link UserInterface}.</p>
 *
 * <p>Most of the controller's state transitions are protected from being called twice in a row: if user clicks on
 * a button like "delete show" multiple times - the controller won't actually try to delete the same show twice.
 * Some actions, that do not trigger view switch, are not protected from that though:</p>
 *
 * <ul>
 *     <li>{@link ApplicationController#crawlShow()}</li>
 *     <li>{@link ApplicationController#shareShowCrawler()}</li>
 * </ul>
 */
public class ApplicationController {
    private final Clipboard clipboard;
    private final Notifications notifications;
    private final ShowPlayer showPlayer;
    private final Shows shows;
    private final ShowsCrawler showsCrawler;
    private final UserInterface userInterface;
    private final DateFormat lastWatchedDateFormat;
    private final EventStack eventStack;

    /**
     * Construct a controller.
     *
     * @param clipboard module that will be used to store data in the system's clipboard
     * @param notifications module that will be used to display system notifications to the user
     * @param showPlayer module that will be used to play shows
     * @param shows module that will be used to access show and show-view related information
     * @param showsCrawler module that will be used to crawl shows and access misc crawling information
     * @param userInterface module that will be treated as a user interface and to which the events will be sent
     */
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

    /**
     * Go to the initial view that displays all the shows, the user can watch.
     */
    public synchronized void viewAllShows() {
        executeTask(new ViewAllShowsTask(shows, lastWatchedDateFormat));
    }

    /**
     * Go to the view that allows user to configure a new show crawler.
     */
    public synchronized void addShow() {
        executeTask(new AddShowTask());
    }

    /**
     * Go to the view that allows user to import a show crawler configuration from a JSON string.
     */
    public synchronized void importShowFromJson() {
        executeTask(new ImportShowFromJsonTask());
    }

    /**
     * Go to the view, that displays details about the specified show.
     *
     * @param showId ID of the show
     */
    public synchronized void selectShow(String showId) {
        executeTask(new SelectShowTask(showId, shows, lastWatchedDateFormat));
    }

    /**
     * Go to the view, that allows user to configure a thumbnail crawler of the currently edited show crawler.
     */
    public synchronized void editThumbnailCrawler() {
        executeTask(new EditCrawlerTask(CrawlerTypes.THUMBNAIL));
    }

    /**
     * Go to the view, that allows user to configure an episode video crawler of the currently edited show crawler.
     */
    public synchronized void editEpisodeVideoCrawler() {
        executeTask(new EditCrawlerTask(CrawlerTypes.EPISODE_VIDEO));
    }

    /**
     * Go to the view, that allows user to configure an episode name crawler of the currently edited show crawler.
     */
    public synchronized void editEpisodeNameCrawler() {
        executeTask(new EditCrawlerTask(CrawlerTypes.EPISODE_NAME));
    }

    /**
     * Save the crawler, that currently being edited (either a thumbnail / video or a name crawler), and proceed back
     * to the show crawler editor view.
     *
     * @param rawCrawler crawler to save
     */
    public synchronized void saveCrawler(String rawCrawler) {
        executeTask(new SaveCrawlerTask(rawCrawler));
    }

    /**
     * Save the show crawler, that is being currently edited, execute it to crawl the corresponding show and
     * go to the show details view of the crawled show.
     *
     * <p>This action should be triggered for both of the following cases:</p>
     *
     * <ul>
     *     <li>the user finishes configuring a new crawler for a new show</li>
     *     <li>the user finishes editing an existing crawler</li>
     * </ul>
     *
     * @param showCrawlerName name of the show, that should be crawled
     */
    public synchronized void saveShowCrawler(String showCrawlerName) {
        executeTask(new SaveShowCrawlerWithoutOverrideTask(showCrawlerName, shows, showsCrawler, lastWatchedDateFormat));
    }

    /**
     * Import the specified show crawler, execute it to crawl the corresponding show and go to the show details view
     * of the crawled show.
     *
     * @param rawShowCrawler whole show crawler configuration to be imported
     */
    public synchronized void importShowCrawler(String rawShowCrawler) {
        executeTask(new ImportShowCrawlerWithoutOverrideTask(rawShowCrawler, shows, showsCrawler, lastWatchedDateFormat));
    }

    /**
     * Run the specified crawler (thumbnail/video/name) without modifying anything and display it's crawling results
     * in the corresponding view.
     *
     * @param rawCrawler crawler configuration to preview
     */
    public synchronized void previewCrawler(String rawCrawler) {
        executeTask(new PreviewCrawlerTask(rawCrawler, showsCrawler));
    }

    /**
     * Display the log of the currently previewed crawler execution.
     */
    public synchronized void viewCrawlLog() {
        executeTask(new ViewCrawlLogTask());
    }

    /**
     * Display detailed information about the specified entry of the currently viewed log.
     *
     * @param logEntryId ID of the log entry to display
     */
    public synchronized void viewLogEntry(int logEntryId) {
        executeTask(new ViewLogEntryTask(logEntryId));
    }

    /**
     * Play the show, show details view of which is being currently displayed, from where the user has left it
     * the last time.
     */
    public synchronized void playShow() {
        executeTask(new PlayShowTask(showPlayer));
    }

    /**
     * Play the show, show details view of which is being currently displayed, starting from the specified episode.
     *
     * @param episodeNumber episode to start the show from
     */
    public synchronized void playShowEpisode(int episodeNumber) {
        executeTask(new PlayShowEpisodeTask(episodeNumber, showPlayer));
    }

    /**
     * Go to the view, that will allow the user to edit the show crawler of the currently selected show.
     */
    public synchronized void editShowCrawler() {
        executeTask(new EditShowCrawlerTask(showsCrawler));
    }

    /**
     * Copy show crawler configuration of the currently selected show as a JSON string and pass it to the system's
     * clipboard, while notifying the user about it using system's notifications.
     */
    public synchronized void shareShowCrawler() {
        executeTask(new ShareShowCrawlerTask(notifications, clipboard, showsCrawler));
    }

    /**
     * Re-crawl the currently selected show and display the updated information about it afterwards.
     */
    public synchronized void crawlShow() {
        executeTask(new CrawlShowTask(showsCrawler, shows, lastWatchedDateFormat));
    }

    /**
     * Go to the view that shows user the last crawling log of the currently selected show.
     */
    public synchronized void viewShowCrawlLog() {
        executeTask(new ViewShowCrawlLogTask(showsCrawler));
    }

    /**
     * Display user a dialog to confirm the operation of clearing the watch history of the currently selected show.
     */
    public synchronized void initiateClearShowWatchHistory() {
        executeTask(new InitiateClearShowWatchHistoryTask());
    }

    /**
     * Display user a dialog to confirm the operation of removing the currently selected show.
     */
    public synchronized void initiateShowRemoval() {
        executeTask(new InitiateShowRemovalTask());
    }

    /**
     * Report watch progress of the currently playing show episode in percentages.
     *
     * @param progress progress of the currently playing show episode in percentages
     */
    public synchronized void reportShowWatchProgress(double progress) {
        executeTask(new ReportShowWatchProgressTask(progress, showPlayer));
    }

    /**
     * Play previous episode relative to the currently playing one.
     */
    public synchronized void playPreviousEpisode() {
        executeTask(new PlayPreviousEpisodeTask(showPlayer));
    }

    /**
     * Play next episode relative to the currently playing one.
     */
    public synchronized void playNextEpisode() {
        executeTask(new PlayNextEpisodeTask(showPlayer));
    }

    /**
     * Clear user's watch history of the currently selected show.
     */
    public synchronized void clearWatchHistory() {
        executeTask(new ClearWatchHistoryTask(shows, lastWatchedDateFormat));
    }

    /**
     * Delete the currently selected show.
     */
    public synchronized void deleteShow() {
        executeTask(new DeleteShowTask(shows, showsCrawler, lastWatchedDateFormat));
    }

    /**
     * Play the show, that was has just finished playing, from the beginning.
     */
    public synchronized void playShowFromTheBeginning() {
        executeTask(new PlayShowFromTheBeginningTask(showPlayer));
    }

    /**
     * Confirm the show override dialog and execute the previously configured show crawler.
     */
    public synchronized void confirmShowOverride() {
        executeTask(new ConfirmShowOverrideTask(shows, showsCrawler, lastWatchedDateFormat));
    }

    /**
     * Go back to the previous view.
     */
    public synchronized void back() {
        executeTask(new BackTask(shows, lastWatchedDateFormat));
    }

    private void executeTask(ControllerTask task) {
        new HandleExceptionsTask(task).execute(eventStack, userInterface);
    }
}
