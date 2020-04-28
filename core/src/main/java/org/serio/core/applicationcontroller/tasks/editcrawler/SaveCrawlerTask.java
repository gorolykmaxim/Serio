package org.serio.core.applicationcontroller.tasks.editcrawler;

import org.serio.core.applicationcontroller.event.EditCrawlerEvent;
import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.IllegalEventStackStateException;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.userinterface.UserInterface;

public class SaveCrawlerTask implements ControllerTask {
    private final String crawler;

    public SaveCrawlerTask(String crawler) {
        this.crawler = crawler;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.pop(EditCrawlerEvent.class).ifPresent(crawlerEvent -> {
            EditShowCrawlerEvent showCrawlerEvent = eventStack.pop(EditShowCrawlerEvent.class)
                    .orElseThrow(() -> new IllegalEventStackStateException(crawlerEvent, EditShowCrawlerEvent.class, eventStack));
            EditShowCrawlerEvent newEvent = showCrawlerEvent.setCrawler(crawlerEvent.getCrawlerType(), crawler);
            eventStack.push(newEvent);
            userInterface.sendEvent(newEvent);
        });
    }
}
