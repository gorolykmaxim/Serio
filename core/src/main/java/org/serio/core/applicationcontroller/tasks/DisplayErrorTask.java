package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.userinterface.UserInterface;

/**
 * Displays the error dialog with the specified error message.
 *
 * <p>Removes the current top event from the stack and throws an error with the specified error message,
 * thus replacing the current top event with the {@link org.serio.core.applicationcontroller.event.ErrorDialogEvent}.
 * If the stack only contains 1 event in it - will only throw an error, thus appending the error
 * event to the stack instead of replacing an existing event.</p>
 */
public class DisplayErrorTask implements ControllerTask {
    private final String errorMessage;

    /**
     * Construct a task.
     *
     * @param errorMessage error message to display
     */
    public DisplayErrorTask(String errorMessage) {
        this.errorMessage = errorMessage;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.popAndPeek();
        throw new RuntimeException(errorMessage);
    }
}
