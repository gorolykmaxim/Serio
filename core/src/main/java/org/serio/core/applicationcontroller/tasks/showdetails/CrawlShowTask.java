package org.serio.core.applicationcontroller.tasks.showdetails;

import org.serio.core.applicationcontroller.event.CrawlingInProgressEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.applicationcontroller.tasks.allshows.SelectShowTask;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.showstorage.Show;
import org.serio.core.userinterface.UserInterface;

/**
 * Crawl the currently selected show and display the updated information in
 * the {@link org.serio.core.userinterface.ViewIds#SHOW_DETAILS} view.
 */
public class CrawlShowTask implements ControllerTask {
    private final ShowsCrawler showsCrawler;
    private final Shows shows;
    private final DateFormat dateFormat;

    /**
     * Construct a task.
     *
     * @param shows module that will be used to get information about the crawled show to display it
     * @param showsCrawler module that will be used to crawl the new show
     * @param dateFormat date format to be applied to show's last watched dates
     */
    public CrawlShowTask(ShowsCrawler showsCrawler, Shows shows, DateFormat dateFormat) {
        this.showsCrawler = showsCrawler;
        this.shows = shows;
        this.dateFormat = dateFormat;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(ShowDetailsEvent.class).ifPresent(lastEvent -> {
            userInterface.sendEvent(new CrawlingInProgressEvent());
            DisplayableShow show = lastEvent.getShow();
            Show updatedShow = showsCrawler.crawlShow(show.getId().toString());
            shows.saveShow(updatedShow);
            eventStack.pop(ShowDetailsEvent.class);
            new SelectShowTask(updatedShow.getId().toString(), shows, dateFormat).execute(eventStack, userInterface);
        });
    }
}
