package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EditCrawlerEvent;
import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.userinterface.UserInterface;

import java.util.Optional;

public class EditCrawlerTask implements ControllerTask {
    private final String crawlerType;

    public EditCrawlerTask(String crawlerType) {
        this.crawlerType = crawlerType;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        Optional<EditShowCrawlerEvent> possibleLastEvent = eventStack.peek(EditShowCrawlerEvent.class);
        if (possibleLastEvent.isPresent()) {
            EditShowCrawlerEvent lastEvent = possibleLastEvent.get();
            EditCrawlerEvent event = new EditCrawlerEvent(crawlerType, lastEvent.getCrawler(crawlerType).orElse(""));
            eventStack.push(event);
            userInterface.sendEvent(event);
        }
    }
}
