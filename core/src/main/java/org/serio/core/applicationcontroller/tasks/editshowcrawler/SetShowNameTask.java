package org.serio.core.applicationcontroller.tasks.editshowcrawler;

import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.userinterface.UserInterface;

/**
 * Attach the current name of the show, crawler of which is being edited right now,
 * to the corresponding {@link EditShowCrawlerEvent}.
 */
public class SetShowNameTask implements ControllerTask {
    private final String showName;

    /**
     * Construct a task.
     *
     * @param showName name of the show, being edited
     */
    public SetShowNameTask(String showName) {
        this.showName = showName;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.pop(EditShowCrawlerEvent.class).ifPresent(event -> {
            eventStack.push(event.setShowName(showName));
        });
    }
}
