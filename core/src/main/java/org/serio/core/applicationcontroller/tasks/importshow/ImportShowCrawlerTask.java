package org.serio.core.applicationcontroller.tasks.importshow;

import org.serio.core.applicationcontroller.event.CrawlingInProgressEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ImportShowFromJsonEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.applicationcontroller.tasks.allshows.SelectShowTask;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.showstorage.Show;
import org.serio.core.userinterface.UserInterface;

/**
 * Import the specified show crawler, crawl the corresponding show and display it in
 * the {@link org.serio.core.userinterface.ViewIds#SHOW_DETAILS} view.
 */
public class ImportShowCrawlerTask implements ControllerTask {
    private final String rawShowCrawler;
    private final Shows shows;
    private final ShowsCrawler showsCrawler;
    private final DateFormat dateFormat;

    /**
     * Construct a task.
     *
     * @param rawShowCrawler body of the show crawler to import
     * @param shows module that will be used to get information about the crawled show to display it
     * @param showsCrawler module that will be used to crawl the new show
     * @param dateFormat date format to be applied to show's last watched dates
     */
    public ImportShowCrawlerTask(String rawShowCrawler, Shows shows, ShowsCrawler showsCrawler, DateFormat dateFormat) {
        this.rawShowCrawler = rawShowCrawler;
        this.shows = shows;
        this.showsCrawler = showsCrawler;
        this.dateFormat = dateFormat;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.isLastEventOfType(ImportShowFromJsonEvent.class)) {
            userInterface.sendEvent(new CrawlingInProgressEvent());
            Show show = showsCrawler.crawlShowAndSaveCrawler(rawShowCrawler);
            shows.saveShow(show);
            eventStack.pop(ImportShowFromJsonEvent.class);
            new SelectShowTask(show.getId().toString(), shows, dateFormat).execute(eventStack, userInterface);
        }
    }
}
