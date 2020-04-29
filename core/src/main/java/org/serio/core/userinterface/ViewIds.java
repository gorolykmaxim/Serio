package org.serio.core.userinterface;

/**
 * IDs of views, that are rendered by the user interface upon receiving {@link ApplicationEvent}.
 */
public class ViewIds {
    /**
     * View that displays all tv shows, available for viewing.
     */
    public static final int ALL_SHOWS = 0;
    /**
     * View that allows creating / editing a show crawler.
     */
    public static final int EDIT_SHOW_CRAWLER = 1;
    /**
     * View that allows importing a complete show crawler configuration via one screen.
     */
    public static final int IMPORT_SHOW_CRAWLER = 2;
    /**
     * View that allows editing an individual crawler.
     */
    public static final int EDIT_CRAWLER = 3;
    /**
     * View that displays crawl preview results.
     */
    public static final int CRAWL_PREVIEW = 4;
    /**
     * View that displays crawl log.
     */
    public static final int CRAWL_LOG = 5;
    /**
     * View that displays details about a specific crawl log entry.
     */
    public static final int CRAWL_LOG_ENTRY_DETAILS = 6;
    /**
     * The view, that is displayed to the user during the crawling process.
     */
    public static final int CRAWLING_IN_PROGRESS = 7;
    /**
     * View that displays details about a specific tv show as well as it's list of episodes.
     */
    public static final int SHOW_DETAILS = 8;
    /**
     * View that plays episodes of a specific show.
     */
    public static final int SHOW_PLAYER = 9;
    /**
     * View that displays a tv show watch history removal confirmation dialog.
     */
    public static final int SHOW_CLEAR_WATCH_HISTORY_DIALOG = 10;
    /**
     * View that displays a tv show removal confirmation dialog.
     */
    public static final int SHOW_DELETE_SHOW_DIALOG = 11;
    /**
     * View that notifies user that the show, he has been watching, is over.
     */
    public static final int SHOW_WATCH_IS_OVER_DIALOG = 12;
    /**
     * View used to display unknown error messages, that can't be handled by the application itself.
     */
    public static final int SHOW_ERROR_DIALOG = 13;
    /**
     * View that notifies user that the name, used while adding / importing a show, is already assigned to
     * an existing show.
     */
    public static final int SHOW_OVERRIDE_DIALOG = 14;
}
