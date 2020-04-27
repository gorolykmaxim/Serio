package org.serio.core.applicationcontroller.event;

import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;

public class ErrorDialogEvent implements ApplicationEvent {
    private final Exception exception;

    public ErrorDialogEvent(Exception exception) {
        if (exception == null) {
            throw new IllegalArgumentException("Exception is not specified");
        }
        this.exception = exception;
    }

    public String getErrorMessage() {
        return exception.getMessage();
    }

    @Override
    public int getViewId() {
        return ViewIds.SHOW_ERROR_DIALOG;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ErrorDialogEvent that = (ErrorDialogEvent) o;
        return Objects.equals(exception, that.exception);
    }

    @Override
    public int hashCode() {
        return Objects.hash(exception);
    }

    @Override
    public String toString() {
        return "ErrorDialogEvent{" +
                "exception=" + exception +
                '}';
    }
}
