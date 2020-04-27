package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EditCrawlerEvent;
import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.IllegalEventStackStateException;
import org.serio.core.userinterface.UserInterface;

import java.util.Optional;

public class SaveCrawlerTask implements ControllerTask {
    private final String crawler;

    public SaveCrawlerTask(String crawler) {
        this.crawler = crawler;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        Optional<EditCrawlerEvent> possibleCrawlerEvent = eventStack.pop(EditCrawlerEvent.class);
        Optional<EditShowCrawlerEvent> possibleShowCrawlerEvent = eventStack.pop(EditShowCrawlerEvent.class);
        if (possibleCrawlerEvent.isPresent() && possibleShowCrawlerEvent.isPresent()) {
            EditCrawlerEvent crawlerEvent = possibleCrawlerEvent.get();
            EditShowCrawlerEvent showCrawlerEvent = possibleShowCrawlerEvent.get();
            EditShowCrawlerEvent newEvent = showCrawlerEvent.setCrawler(crawlerEvent.getCrawlerType(), crawler);
            eventStack.push(newEvent);
            userInterface.sendEvent(newEvent);
        } else if (possibleCrawlerEvent.isPresent()) {
            throw new IllegalEventStackStateException(possibleCrawlerEvent.get(), EditShowCrawlerEvent.class, eventStack);
        }
    }
}
