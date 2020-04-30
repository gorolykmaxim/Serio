package org.serio.core.applicationcontroller.tasks.editcrawler;

import org.serio.core.applicationcontroller.event.EditCrawlerEvent;
import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.IllegalEventStackStateException;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.userinterface.UserInterface;

/**
 * Attach the currently edited crawler to the currently edited show crawler, and go back to
 * the {@link org.serio.core.userinterface.ViewIds#EDIT_SHOW_CRAWLER} view.
 */
public class SaveCrawlerTask implements ControllerTask {
    private final String crawler;

    /**
     * Construct a task.
     *
     * @param crawler crawler to attach to the currently edited show crawler
     */
    public SaveCrawlerTask(String crawler) {
        this.crawler = crawler;
    }

    /**
     * {@inheritDoc}
     */
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
