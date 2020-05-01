package org.serio.core.applicationcontroller.tasks.importshow;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ImportShowFromJsonEvent;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.userinterface.UserInterface;

/**
 * Attach the current version of the show crawler, that will be imported from json,
 * to the corresponding {@link ImportShowFromJsonEvent}.
 */
public class SetShowCrawlerTask implements ControllerTask {
    private final String showCrawler;

    /**
     * Construct a task.
     *
     * @param showCrawler show crawler, that is going to be imported
     */
    public SetShowCrawlerTask(String showCrawler) {
        this.showCrawler = showCrawler;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.pop(ImportShowFromJsonEvent.class).ifPresent(event -> {
            eventStack.push(new ImportShowFromJsonEvent(showCrawler));
        });
    }
}
