package org.serio.core.applicationcontroller.tasks.showdetails;

import org.serio.core.applicationcontroller.event.CrawlLogEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

import java.util.Collections;
import java.util.List;

/**
 * Open the {@link org.serio.core.userinterface.ViewIds#CRAWL_LOG} and display the last crawl log of the currently
 * selected show.
 */
public class ViewShowCrawlLogTask implements ControllerTask {
    private final ShowsCrawler showsCrawler;

    /**
     * Construct a task.
     *
     * @param showsCrawler module that will be used to obtain the last crawl log from
     */
    public ViewShowCrawlLogTask(ShowsCrawler showsCrawler) {
        this.showsCrawler = showsCrawler;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(ShowDetailsEvent.class).ifPresent(lastEvent -> {
            DisplayableShow show = lastEvent.getShow();
            List<CrawlLogEntry> lastCrawlLog = showsCrawler.getLastCrawlingLogOfShow(show.getId().toString())
                    .orElse(Collections.emptyList());
            CrawlLogEvent event = new CrawlLogEvent(lastCrawlLog);
            eventStack.push(event);
            userInterface.sendEvent(event);
        });
    }
}
