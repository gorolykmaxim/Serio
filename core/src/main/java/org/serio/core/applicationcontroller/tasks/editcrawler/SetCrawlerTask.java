package org.serio.core.applicationcontroller.tasks.editcrawler;

import org.serio.core.applicationcontroller.event.EditCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.userinterface.UserInterface;

/**
 * Attach the current version of the crawler, that is being edited right now,
 * to the corresponding {@link EditCrawlerEvent}.
 */
public class SetCrawlerTask implements ControllerTask {
    private final String crawler;

    /**
     * Construct a task.
     *
     * @param crawler body of the crawler to attach
     */
    public SetCrawlerTask(String crawler) {
        this.crawler = crawler;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.pop(EditCrawlerEvent.class).ifPresent(event -> {
            eventStack.push(event.setCrawler(crawler));
        });
    }
}
