package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.CrawlLogEvent;
import org.serio.core.applicationcontroller.event.CrawlPreviewEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.userinterface.UserInterface;

import java.util.Optional;

public class ViewCrawlLogTask implements ControllerTask {
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        Optional<CrawlPreviewEvent> possibleLastEvent = eventStack.pop(CrawlPreviewEvent.class);
        if (possibleLastEvent.isPresent()) {
            CrawlPreviewEvent lastEvent = possibleLastEvent.get();
            CrawlLogEvent event = new CrawlLogEvent(lastEvent.getCrawlerType(), lastEvent.getCrawlingResult().getLog());
            eventStack.push(event);
            userInterface.sendEvent(event);
        }
    }
}
