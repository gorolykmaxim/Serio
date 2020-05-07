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
import org.serio.core.applicationcontroller.tasks.editcrawler.DisplayCrawlerHelpTask;
import org.serio.core.applicationcontroller.tasks.editcrawler.PreviewCrawlerTask;
import org.serio.core.applicationcontroller.tasks.editcrawler.SaveCrawlerTask;
import org.serio.core.applicationcontroller.tasks.editcrawler.SetCrawlerTask;
import org.serio.core.applicationcontroller.tasks.editshowcrawler.EditCrawlerTask;
import org.serio.core.applicationcontroller.tasks.editshowcrawler.SaveShowCrawlerWithoutOverrideTask;
import org.serio.core.applicationcontroller.tasks.editshowcrawler.SetShowNameTask;
import org.serio.core.applicationcontroller.tasks.importshow.ImportShowCrawlerWithoutOverrideTask;
import org.serio.core.applicationcontroller.tasks.importshow.SetShowCrawlerTask;
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
 * {@link ApplicationController} that uses {@link EventStack} to keep track of all events, send to the UI.
 *
 * <p>{@link EventStackApplicationController} keeps track of all the events sent to the {@link UserInterface}. Those events
 * are kept in a corresponding stack, which represents the current tree of opened views: the view that is
 * currently displayed to the user is on top of the stack and the view, that was displayed previously and can
 * be switched back to is right below it in the stack. Each view in the stack is represented by the corresponding
 * {@link org.serio.core.userinterface.ApplicationEvent}, that was sent to the {@link UserInterface} to render
 * that view.</p>
 *
 * <p>{@link EventStackApplicationController} keeps all the necessary context information in it's events. Thus, the controller
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
 *     <li>{@link EventStackApplicationController#crawlShow()}</li>
 *     <li>{@link EventStackApplicationController#shareShowCrawler()}</li>
 * </ul>
 */
public class EventStackApplicationController implements ApplicationController {
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
    public EventStackApplicationController(Clipboard clipboard, Notifications notifications, ShowPlayer showPlayer, Shows shows,
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
     * {@inheritDoc}
     */
    @Override
    public synchronized void viewAllShows() {
        executeTask(new ViewAllShowsTask(shows, lastWatchedDateFormat));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void addShow() {
        executeTask(new AddShowTask());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void importShowFromJson() {
        executeTask(new ImportShowFromJsonTask());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void setShowCrawler(String rawShowCrawler) {
        executeTask(new SetShowCrawlerTask(rawShowCrawler));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void selectShow(String showId) {
        executeTask(new SelectShowTask(showId, shows, lastWatchedDateFormat));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void editThumbnailCrawler() {
        executeTask(new EditCrawlerTask(CrawlerTypes.THUMBNAIL));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void editEpisodeVideoCrawler() {
        executeTask(new EditCrawlerTask(CrawlerTypes.EPISODE_VIDEO));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void editEpisodeNameCrawler() {
        executeTask(new EditCrawlerTask(CrawlerTypes.EPISODE_NAME));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void setCrawler(String rawCrawler) {
        executeTask(new SetCrawlerTask(rawCrawler));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void saveCrawler() {
        executeTask(new SaveCrawlerTask());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void setShowName(String showName) {
        executeTask(new SetShowNameTask(showName));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void saveShowCrawler() {
        executeTask(new SaveShowCrawlerWithoutOverrideTask(shows, showsCrawler, lastWatchedDateFormat));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void importShowCrawler() {
        executeTask(new ImportShowCrawlerWithoutOverrideTask(shows, showsCrawler, lastWatchedDateFormat));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void previewCrawler() {
        executeTask(new PreviewCrawlerTask(showsCrawler));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void viewCrawlLog() {
        executeTask(new ViewCrawlLogTask());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void viewLogEntry(int logEntryId) {
        executeTask(new ViewLogEntryTask(logEntryId));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void playShow() {
        executeTask(new PlayShowTask(showPlayer));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void playShowEpisode(int episodeNumber) {
        executeTask(new PlayShowEpisodeTask(episodeNumber, showPlayer));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void editShowCrawler() {
        executeTask(new EditShowCrawlerTask(showsCrawler));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void shareShowCrawler() {
        executeTask(new ShareShowCrawlerTask(notifications, clipboard, showsCrawler));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void crawlShow() {
        executeTask(new CrawlShowTask(showsCrawler, shows, lastWatchedDateFormat));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void viewShowCrawlLog() {
        executeTask(new ViewShowCrawlLogTask(showsCrawler));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void initiateClearShowWatchHistory() {
        executeTask(new InitiateClearShowWatchHistoryTask());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void initiateShowRemoval() {
        executeTask(new InitiateShowRemovalTask());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void reportShowWatchProgress(double progress) {
        executeTask(new ReportShowWatchProgressTask(progress, showPlayer));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void playPreviousEpisode() {
        executeTask(new PlayPreviousEpisodeTask(showPlayer));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void playNextEpisode() {
        executeTask(new PlayNextEpisodeTask(showPlayer));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void clearWatchHistory() {
        executeTask(new ClearWatchHistoryTask(shows, lastWatchedDateFormat));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void deleteShow() {
        executeTask(new DeleteShowTask(shows, showsCrawler, lastWatchedDateFormat));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void playShowFromTheBeginning() {
        executeTask(new PlayShowFromTheBeginningTask(showPlayer));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void confirmShowOverride() {
        executeTask(new ConfirmShowOverrideTask(shows, showsCrawler, lastWatchedDateFormat));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void displayError(String error) {
        executeTask(new DisplayErrorTask(error));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void displayCrawlerHelp() {
        executeTask(new DisplayCrawlerHelpTask());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public synchronized void back() {
        executeTask(new BackTask(shows, lastWatchedDateFormat));
    }

    private void executeTask(ControllerTask task) {
        new HandleExceptionsTask(task).execute(eventStack, userInterface);
    }
}
