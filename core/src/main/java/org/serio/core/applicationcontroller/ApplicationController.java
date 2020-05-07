package org.serio.core.applicationcontroller;

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
 */
public interface ApplicationController {
    /**
     * Go to the initial view that displays all the shows, the user can watch.
     */
    void viewAllShows();

    /**
     * Go to the view that allows user to configure a new show crawler.
     */
    void addShow();

    /**
     * Go to the view that allows user to import a show crawler configuration from a JSON string.
     */
    void importShowFromJson();

    /**
     * Persist the currently specified show crawler, being imported from json.
     *
     * @param rawShowCrawler show crawler to persist
     */
    void setShowCrawler(String rawShowCrawler);

    /**
     * Go to the view, that displays details about the specified show.
     *
     * @param showId ID of the show
     */
    void selectShow(String showId);

    /**
     * Go to the view, that allows user to configure a thumbnail crawler of the currently edited show crawler.
     */
    void editThumbnailCrawler();

    /**
     * Go to the view, that allows user to configure an episode video crawler of the currently edited show crawler.
     */
    void editEpisodeVideoCrawler();

    /**
     * Go to the view, that allows user to configure an episode name crawler of the currently edited show crawler.
     */
    void editEpisodeNameCrawler();

    /**
     * Persist the crawler, that is currently being edited.
     *
     * @param rawCrawler crawler to persist
     */
    void setCrawler(String rawCrawler);

    /**
     * Save the crawler, that currently being edited (either a thumbnail / video or a name crawler), and proceed back
     * to the show crawler editor view.
     */
    void saveCrawler();

    /**
     * Persist the name of the show, crawler of which is being edited right now.
     *
     * @param showName show name to persist
     */
    void setShowName(String showName);

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
     */
    void saveShowCrawler();

    /**
     * Import the currently persisted show crawler, execute it to crawl the corresponding show and go to the
     * show details view of the crawled show.
     */
    void importShowCrawler();

    /**
     * Run the crawler (thumbnail/video/name), that is currently being edited, without modifying anything
     * and display it's crawling results in the corresponding view.
     */
    void previewCrawler();

    /**
     * Display the log of the currently previewed crawler execution.
     */
    void viewCrawlLog();

    /**
     * Display detailed information about the specified entry of the currently viewed log.
     *
     * @param logEntryId ID of the log entry to display
     */
    void viewLogEntry(int logEntryId);

    /**
     * Play the show, show details view of which is being currently displayed, from where the user has left it
     * the last time.
     */
    void playShow();

    /**
     * Play the show, show details view of which is being currently displayed, starting from the specified episode.
     *
     * @param episodeNumber episode to start the show from
     */
    void playShowEpisode(int episodeNumber);

    /**
     * Go to the view, that will allow the user to edit the show crawler of the currently selected show.
     */
    void editShowCrawler();

    /**
     * Copy show crawler configuration of the currently selected show as a JSON string and pass it to the system's
     * clipboard, while notifying the user about it using system's notifications.
     */
    void shareShowCrawler();

    /**
     * Re-crawl the currently selected show and display the updated information about it afterwards.
     */
    void crawlShow();

    /**
     * Go to the view that shows user the last crawling log of the currently selected show.
     */
    void viewShowCrawlLog();

    /**
     * Display user a dialog to confirm the operation of clearing the watch history of the currently selected show.
     */
    void initiateClearShowWatchHistory();

    /**
     * Display user a dialog to confirm the operation of removing the currently selected show.
     */
    void initiateShowRemoval();

    /**
     * Report watch progress of the currently playing show episode in percentages.
     *
     * @param progress progress of the currently playing show episode in percentages
     */
    void reportShowWatchProgress(double progress);

    /**
     * Play previous episode relative to the currently playing one.
     */
    void playPreviousEpisode();

    /**
     * Play next episode relative to the currently playing one.
     */
    void playNextEpisode();

    /**
     * Clear user's watch history of the currently selected show.
     */
    void clearWatchHistory();

    /**
     * Delete the currently selected show.
     */
    void deleteShow();

    /**
     * Play the show, that was has just finished playing, from the beginning.
     */
    void playShowFromTheBeginning();

    /**
     * Confirm the show override dialog and execute the previously configured show crawler.
     */
    void confirmShowOverride();

    /**
     * Display the specified error in the error dialog, while discarding the currently displayed view.
     *
     * @param error error message to display
     */
    void displayError(String error);

    /**
     * Go to the view with help on how to configure a crawler.
     */
    void displayCrawlerHelp();

    /**
     * Go back to the previous view.
     */
    void back();
}
