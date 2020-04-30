package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.userinterface.UserInterface;

/**
 * A task the {@link org.serio.core.applicationcontroller.ApplicationController} executes each time the user interacts
 * with the user interface.
 *
 * <p>Each unique kind of user interface interaction should have it's own implementation
 * of the {@link ControllerTask}.</p>
 */
public interface ControllerTask {
    /**
     * Execute this task against the specified event stack and the user interface.
     *
     * <p>Modify the event stack if the view hierarchy should be changed or an additional context information
     * should be applied to the one of the events, currently stored in the stack. Send events to the user interface
     * if necessary.</p>
     *
     * <p>If the task should not be executed with the current state of the specified event stack - this
     * call will be ignored.</p>
     *
     * @param eventStack event stack to operate on
     * @param userInterface user interface to send events to if necessary
     */
    void execute(EventStack eventStack, UserInterface userInterface);
}
