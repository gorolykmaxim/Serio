package org.serio.core.notifications;

/**
 * Displays notifications to the user via platform-specific system services.
 */
public interface Notifications {
    /**
     * Display specified text as a short simple notification.
     *
     * @param text text to display
     */
    void displayTextAsNotification(String text);
}
