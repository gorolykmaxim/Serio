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
 * Preview the crawler, that is currently being edited, by executing it and showing it's output in
 * {@link org.serio.core.userinterface.ViewIds#CRAWL_PREVIEW} view.
 */
public class PreviewCrawlerTask implements ControllerTask {
    private final ShowsCrawler showsCrawler;

    /**
     * Construct a task.
     *
     * @param showsCrawler module that will be used to preview the crawler
     */
    public PreviewCrawlerTask(ShowsCrawler showsCrawler) {
        this.showsCrawler = showsCrawler;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(EditCrawlerEvent.class).ifPresent(lastEvent -> {
            String rawCrawler = lastEvent.getCrawler();
            userInterface.sendEvent(new CrawlingInProgressEvent());
            CrawlingResult crawlingResult = showsCrawler.previewCrawler(rawCrawler);
            CrawlPreviewEvent event = new CrawlPreviewEvent(lastEvent.getCrawlerType(), crawlingResult);
            eventStack.push(event);
            userInterface.sendEvent(event);
        });
    }
}
