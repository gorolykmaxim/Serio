package org.serio.core;

import org.serio.core.applicationcontroller.ApplicationController;
import org.serio.core.clipboard.Clipboard;
import org.serio.core.httpclient.HttpClient;
import org.serio.core.notifications.Notifications;
import org.serio.core.showcrawlerlogstorage.ShowCrawlerLogStorage;
import org.serio.core.showcrawlerstorage.ShowCrawlerStorage;
import org.serio.core.showplayer.ShowPlayer;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.showstorage.ShowStorage;
import org.serio.core.userinterface.UserInterface;
import org.serio.core.watchhistory.WatchHistory;

/**
 * The core of the Serio application.
 *
 * <p>The cross-platform part of the code, that contains the primary business logic of the application.</p>
 */
public class Core {
    private final ApplicationController controller;

    /**
     * Construct the core.
     *
     * @param clipboard platform-specific implementation of the {@link Clipboard} module
     * @param httpClient platform-specific implementation of the {@link HttpClient} module
     * @param notifications platform-specific implementation of the {@link Notifications} module
     * @param showCrawlerLogStorage platform-specific implementation of the {@link ShowCrawlerLogStorage} module
     * @param showCrawlerStorage platform-specific implementation of the {@link ShowCrawlerStorage} module
     * @param showStorage platform-specific implementation of the {@link ShowStorage} module
     * @param userInterface platform-specific implementation of the {@link UserInterface} module
     * @param watchHistory platform-specific implementation of the {@link WatchHistory} module
     */
    public Core(Clipboard clipboard, HttpClient httpClient, Notifications notifications,
                ShowCrawlerLogStorage showCrawlerLogStorage, ShowCrawlerStorage showCrawlerStorage,
                ShowStorage showStorage, UserInterface userInterface, WatchHistory watchHistory) {
        Shows shows = new Shows(showStorage, watchHistory);
        ShowsCrawler showsCrawler = new ShowsCrawler(showCrawlerStorage, showCrawlerLogStorage, httpClient, 500);
        ShowPlayer showPlayer = new ShowPlayer(shows);
        controller = new ApplicationController(clipboard, notifications, showPlayer, shows, showsCrawler, userInterface);
    }

    /**
     * Initialize and return the module, that can be used by the user interface to interact with the rest of the
     * core's modules.
     *
     * @return initialized application controller
     */
    public ApplicationController initializeApplicationController() {
        controller.viewAllShows();
        return controller;
    }
}
