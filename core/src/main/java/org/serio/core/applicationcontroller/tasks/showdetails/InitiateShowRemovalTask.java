package org.serio.core.applicationcontroller.tasks.showdetails;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.userinterface.UserInterface;

/**
 * Display the {@link org.serio.core.userinterface.ViewIds#SHOW_DELETE_SHOW_DIALOG} view.
 */
public class InitiateShowRemovalTask implements ControllerTask {
    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(ShowDetailsEvent.class).ifPresent(lastEvent -> {
            DisplayableShow show = lastEvent.getShow();
            ShowDialogEvent event = ShowDialogEvent.deleteShowDialog(show.getName());
            eventStack.push(event);
            userInterface.sendEvent(event);
        });
    }
}
