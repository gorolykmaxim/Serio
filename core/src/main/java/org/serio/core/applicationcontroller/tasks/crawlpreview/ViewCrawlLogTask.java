package org.serio.core.applicationcontroller.tasks.crawlpreview;

import org.serio.core.applicationcontroller.event.CrawlLogEvent;
import org.serio.core.applicationcontroller.event.CrawlPreviewEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.userinterface.UserInterface;

public class ViewCrawlLogTask implements ControllerTask {
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(CrawlPreviewEvent.class).ifPresent(lastEvent -> {
            CrawlLogEvent event = new CrawlLogEvent(lastEvent.getCrawlerType(), lastEvent.getCrawlingResult().getLog());
            eventStack.push(event);
            userInterface.sendEvent(event);
        });
    }
}
