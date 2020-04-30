package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.ErrorDialogEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.userinterface.UserInterface;

/**
 * Special kind of {@link ControllerTask} that wraps around the specified task and catches all of it's exceptions.
 *
 * <p>If an exception is caught by this task - a corresponding {@link ErrorDialogEvent} gets pushed to the stack
 * and send to the user interface.</p>
 */
public class HandleExceptionsTask implements ControllerTask {
    private final ControllerTask task;

    /**
     * Construct a task.
     *
     * @param task actual task to wrap around
     */
    public HandleExceptionsTask(ControllerTask task) {
        this.task = task;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        try {
            task.execute(eventStack, userInterface);
        } catch (Exception e) {
            ErrorDialogEvent event = new ErrorDialogEvent(e);
            eventStack.push(event);
            userInterface.sendEvent(event);
        }
    }
}
