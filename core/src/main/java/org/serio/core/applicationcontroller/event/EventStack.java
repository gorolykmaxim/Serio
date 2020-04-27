package org.serio.core.applicationcontroller.event;

import org.serio.core.userinterface.ApplicationEvent;

import java.util.Optional;
import java.util.Stack;

public class EventStack {
    private final Stack<ApplicationEvent> events;

    public EventStack() {
        events = new Stack<>();
    }

    public void clear() {
        events.clear();
    }

    public void push(ApplicationEvent event) {
        events.push(event);
    }

    public boolean isLastEventOfType(Class<?> expectedEventType) {
        return peek(expectedEventType).isPresent();
    }

    public Optional<ApplicationEvent> popAndPeek() {
        if (events.size() > 1) {
            events.pop();
            return Optional.of(events.peek());
        } else {
            return Optional.empty();
        }
    }

    public <T> Optional<T> peek(Class<T> expectedEventType) {
        return convertEvent(expectedEventType, events.peek());
    }

    public <T> Optional<T> pop(Class<T> expectedEventType) {
        return isLastEventOfType(expectedEventType) ? convertEvent(expectedEventType, events.pop()) : Optional.empty();
    }

    private <T> Optional<T> convertEvent(Class<T> expectedEventType, ApplicationEvent event) {
        return expectedEventType.equals(event.getClass()) ? Optional.of((T) event) : Optional.empty();
    }

    @Override
    public String toString() {
        return "EventStack{" +
                "events=" + events +
                '}';
    }
}
