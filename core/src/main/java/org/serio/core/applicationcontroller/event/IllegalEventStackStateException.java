package org.serio.core.applicationcontroller.event;

import org.serio.core.userinterface.ApplicationEvent;

public class IllegalEventStackStateException extends RuntimeException {
    public IllegalEventStackStateException(ApplicationEvent event, EventStack eventStack) {
        super(String.format("%s has unexpected previous event. Details: %s", event, eventStack));
    }

    public IllegalEventStackStateException(ApplicationEvent event, Class<?> expectedPreviousEventType, EventStack eventStack) {
        super(String.format("%s's previous event was expected to be of type %s but was not. Details: %s", event, expectedPreviousEventType.getName(), eventStack));
    }
}
