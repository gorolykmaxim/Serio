package org.serio.core.applicationcontroller.tasks.editshowcrawler;

import org.apache.commons.lang3.StringUtils;
import org.serio.core.applicationcontroller.event.*;
import org.serio.core.applicationcontroller.model.CrawlerTypes;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.applicationcontroller.tasks.RePopulateAllShows;
import org.serio.core.applicationcontroller.tasks.allshows.SelectShowTask;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.showstorage.Show;
import org.serio.core.userinterface.UserInterface;

/**
 * Save the show crawler, that is currently being edited, crawl the corresponding show and display it in
 * the {@link org.serio.core.userinterface.ViewIds#SHOW_DETAILS} view.
 *
 * <p>Since the crawling process may take some time, during it the {@link CrawlingInProgressEvent} will be
 * displayed to the user.</p>
 */
public class SaveShowCrawlerTask implements ControllerTask {
    private final String showName;
    private final Shows shows;
    private final ShowsCrawler showsCrawler;
    private final DateFormat dateFormat;

    /**
     * Construct a task.
     *
     * @param showName name of the show, crawled by the show crawler
     * @param shows module that will be used to get information about the crawled show to display it
     * @param showsCrawler module that will be used to crawl the new show
     * @param dateFormat date format to be applied to show's last watched dates
     */
    public SaveShowCrawlerTask(String showName, Shows shows, ShowsCrawler showsCrawler, DateFormat dateFormat) {
        this.showName = showName;
        this.shows = shows;
        this.showsCrawler = showsCrawler;
        this.dateFormat = dateFormat;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(EditShowCrawlerEvent.class).ifPresent(lastEvent -> {
            userInterface.sendEvent(new CrawlingInProgressEvent());
            Show show = showsCrawler.crawlShowAndSaveCrawler(
                    StringUtils.defaultString(showName, lastEvent.getShowName().orElse(null)),
                    lastEvent.getCrawler(CrawlerTypes.THUMBNAIL).orElse(null),
                    lastEvent.getCrawler(CrawlerTypes.EPISODE_VIDEO).orElse(null),
                    lastEvent.getCrawler(CrawlerTypes.EPISODE_NAME).orElse(null)
            );
            shows.saveShow(show);
            new RePopulateAllShows(shows, dateFormat).execute(eventStack, userInterface);
            eventStack.pop(EditShowCrawlerEvent.class);
            if (eventStack.isLastEventOfType(AllShowsEvent.class) || eventStack.pop(ShowDetailsEvent.class).isPresent()) {
                new SelectShowTask(show.getId().toString(), shows, dateFormat).execute(eventStack, userInterface);
            } else {
                throw new IllegalEventStackStateException(lastEvent, eventStack);
            }
        });
    }
}
