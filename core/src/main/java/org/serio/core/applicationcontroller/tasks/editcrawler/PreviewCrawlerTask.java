package org.serio.core.applicationcontroller.tasks.editcrawler;

import org.serio.core.applicationcontroller.event.CrawlPreviewEvent;
import org.serio.core.applicationcontroller.event.CrawlingInProgressEvent;
import org.serio.core.applicationcontroller.event.EditCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.showscrawler.CrawlingResult;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

/**
 * Preview the specified crawler by executing it and showing it's output in
 * {@link org.serio.core.userinterface.ViewIds#CRAWL_PREVIEW} view.
 */
public class PreviewCrawlerTask implements ControllerTask {
    private final String rawCrawler;
    private final ShowsCrawler showsCrawler;

    /**
     * Construct a task.
     *
     * @param rawCrawler body of the crawler to preview
     * @param showsCrawler module that will be used to preview the crawler
     */
    public PreviewCrawlerTask(String rawCrawler, ShowsCrawler showsCrawler) {
        this.rawCrawler = rawCrawler;
        this.showsCrawler = showsCrawler;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.pop(EditCrawlerEvent.class).ifPresent(lastEvent -> {
            // Save the specified crawler so that when the user will come back to the edit crawler view
            // the crawler will still be displayed.
            eventStack.push(new EditCrawlerEvent(lastEvent.getCrawlerType(), rawCrawler));
            userInterface.sendEvent(new CrawlingInProgressEvent());
            CrawlingResult crawlingResult = showsCrawler.previewCrawler(rawCrawler);
            CrawlPreviewEvent event = new CrawlPreviewEvent(lastEvent.getCrawlerType(), crawlingResult);
            eventStack.push(event);
            userInterface.sendEvent(event);
        });
    }
}
