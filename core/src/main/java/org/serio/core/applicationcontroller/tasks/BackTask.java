package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.userinterface.UserInterface;

/**
 * Handles the "back" user interface operation where the currently displayed view should be discarded an the previously
 * displayed view should be displayed once again.
 *
 * <p>Simply pops the current head of the event stack and sends the new head of the stack to the UI.</p>
 */
public class BackTask implements ControllerTask {
    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.popAndPeek().ifPresent(userInterface::sendEvent);
    }
}
