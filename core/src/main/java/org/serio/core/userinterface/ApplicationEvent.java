package org.serio.core.userinterface;

/**
 * An event, that is passed from the core of the application to the user interface.
 *
 * <p>Upon receiving an event, the user interface draws a view, that corresponds to the arrived event.</p>
 */
public interface ApplicationEvent {
    /**
     * Get ID of the view, that should be rendered by the user interface upon receiving this event.
     *
     * @return ID of the view, that corresponds to this event
     */
    int getViewId();
}
