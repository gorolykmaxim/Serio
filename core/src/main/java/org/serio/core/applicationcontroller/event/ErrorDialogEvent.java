package org.serio.core.applicationcontroller.event;

import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;

/**
 * Event that will trigger rendering of {@link ViewIds#SHOW_ERROR_DIALOG} view.
 */
public class ErrorDialogEvent implements ApplicationEvent {
    private final Exception exception;

    /**
     * Construct an event.
     *
     * @param exception error, information about which should be rendered
     */
    public ErrorDialogEvent(Exception exception) {
        if (exception == null) {
            throw new IllegalArgumentException("Exception is not specified");
        }
        this.exception = exception;
    }

    /**
     * Get error message, that should be displayed to the user.
     *
     * @return error message to display
     */
    public String getErrorMessage() {
        return exception.getMessage();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getViewId() {
        return ViewIds.SHOW_ERROR_DIALOG;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ErrorDialogEvent that = (ErrorDialogEvent) o;
        return Objects.equals(exception, that.exception);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(exception);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "ErrorDialogEvent{" +
                "exception=" + exception +
                '}';
    }
}
