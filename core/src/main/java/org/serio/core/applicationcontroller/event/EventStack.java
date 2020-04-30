package org.serio.core.applicationcontroller.event;

import org.serio.core.userinterface.ApplicationEvent;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Stack;

/**
 * Stack of {@link ApplicationEvent}s.
 *
 * <p>Represents the order in which currently opened views are order on top of each other. On top of the stack
 * lays the event, that corresponds to the view that should be currently displayed on the screen. Next goes the
 * event, view of which should be displayed when the user closes the current view and so on. On the bottom of the
 * stack lays the root view of the application.</p>
 */
public class EventStack {
    private final Stack<ApplicationEvent> events;

    /**
     * Construct a stack.
     */
    public EventStack() {
        events = new Stack<>();
    }

    /**
     * Clear the stack by removing all of it's events.
     */
    public void clear() {
        events.clear();
    }

    /**
     * Put the specified event on top of the stack.
     *
     * @param event event to push
     */
    public void push(ApplicationEvent event) {
        events.push(event);
    }

    /**
     * Check if the event, that is currently on top of the stack, is of the specified type.
     *
     * @param expectedEventType expected type of the top event
     * @return true if the top event of the stack has the specified type. Will be false if the top event of the stack
     * has different type or if the stack is empty.
     */
    public boolean isLastEventOfType(Class<?> expectedEventType) {
        return peek(expectedEventType).isPresent();
    }

    /**
     * Pop and peek operations, combined in a safe manner.
     *
     * <p>Pop the current top event from the stack and return the next event, laying right under it.
     * If the stack has only one event left - the event won't get popped and nothing will be returned.</p>
     *
     * @return new top event of the stack. Can be empty if the stack is empty or has only one event in it.
     */
    public Optional<ApplicationEvent> popAndPeek() {
        if (events.size() > 1) {
            events.pop();
            return Optional.of(events.peek());
        } else {
            return Optional.empty();
        }
    }

    /**
     * Create copy of the stack, represented as a list.
     *
     * @return list copy of this stack
     */
    public List<ApplicationEvent> toList() {
        return new ArrayList<>(events);
    }

    /**
     * Get the top event from the stack, without removing it, IF the top event has the specified type. If the top
     * event of the stack has a different type - nothing will get returned.
     *
     * @param expectedEventType expected type of the top event
     * @param <T>
     * @return top event of the stack of the specified type. Can be empty if the top event of the stack has a different
     * type or if the stack is empty.
     */
    public <T> Optional<T> peek(Class<T> expectedEventType) {
        return convertEvent(expectedEventType, events.peek());
    }

    /**
     * Get the top event from the stack, while removing it from the stack, IF the top event has the specified type.
     * If the top event of the stack has a different type - it won't get removed from the stack and nothing will get
     * returned.
     *
     * @param expectedEventType expected type of the top event
     * @param <T>
     * @return top event of the stack of the specified type. Can be empty if the top event of the stack has a different
     * type or if the stack is empty
     */
    public <T> Optional<T> pop(Class<T> expectedEventType) {
        return isLastEventOfType(expectedEventType) ? convertEvent(expectedEventType, events.pop()) : Optional.empty();
    }

    private <T> Optional<T> convertEvent(Class<T> expectedEventType, ApplicationEvent event) {
        return expectedEventType.equals(event.getClass()) ? Optional.of((T) event) : Optional.empty();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "EventStack{" +
                "events=" + events +
                '}';
    }
}
