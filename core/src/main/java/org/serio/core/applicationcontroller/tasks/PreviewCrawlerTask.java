package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.CrawlPreviewEvent;
import org.serio.core.applicationcontroller.event.CrawlingInProgressEvent;
import org.serio.core.applicationcontroller.event.EditCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.showscrawler.CrawlingResult;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

public class PreviewCrawlerTask implements ControllerTask {
    private final String rawCrawler;
    private final ShowsCrawler showsCrawler;

    public PreviewCrawlerTask(String rawCrawler, ShowsCrawler showsCrawler) {
        this.rawCrawler = rawCrawler;
        this.showsCrawler = showsCrawler;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(EditCrawlerEvent.class).ifPresent(lastEvent -> {
            userInterface.sendEvent(new CrawlingInProgressEvent());
            CrawlingResult crawlingResult = showsCrawler.previewCrawler(rawCrawler);
            CrawlPreviewEvent event = new CrawlPreviewEvent(lastEvent.getCrawlerType(), crawlingResult);
            eventStack.push(event);
            userInterface.sendEvent(event);
        });
    }
}
