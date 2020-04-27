package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.ErrorDialogEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.userinterface.UserInterface;

public class HandleExceptionsTask implements ControllerTask {
    private final ControllerTask task;

    public HandleExceptionsTask(ControllerTask task) {
        this.task = task;
    }

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
