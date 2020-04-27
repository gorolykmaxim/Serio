package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.userinterface.UserInterface;

public class BackTask implements ControllerTask {
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.popAndPeek().ifPresent(userInterface::sendEvent);
    }
}
