package org.serio.core.userinterface;

/**
 * Abstraction of a platform-specific user interface.
 */
public interface UserInterface {
    /**
     * Send event to the user interface with the information about the view, that should be rendered.
     *
     * @param event application event to send
     */
    void sendEvent(ApplicationEvent event);
}
