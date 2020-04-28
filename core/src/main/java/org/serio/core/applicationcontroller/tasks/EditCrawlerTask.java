package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EditCrawlerEvent;
import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.userinterface.UserInterface;

public class EditCrawlerTask implements ControllerTask {
    private final String crawlerType;

    public EditCrawlerTask(String crawlerType) {
        this.crawlerType = crawlerType;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(EditShowCrawlerEvent.class).ifPresent(lastEvent -> {
            EditCrawlerEvent event = new EditCrawlerEvent(crawlerType, lastEvent.getCrawler(crawlerType).orElse(""));
            eventStack.push(event);
            userInterface.sendEvent(event);
        });
    }
}
