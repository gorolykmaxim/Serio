package org.serio.core.applicationcontroller.tasks.allshows;

import org.serio.core.applicationcontroller.event.AllShowsEvent;
import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.userinterface.UserInterface;

/**
 * Opens the {@link org.serio.core.userinterface.ViewIds#EDIT_SHOW_CRAWLER} view to create a crawler for a new show.
 */
public class AddShowTask implements ControllerTask {
    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.isLastEventOfType(AllShowsEvent.class)) {
            EditShowCrawlerEvent event = new EditShowCrawlerEvent();
            eventStack.push(event);
            userInterface.sendEvent(event);
        }
    }
}
