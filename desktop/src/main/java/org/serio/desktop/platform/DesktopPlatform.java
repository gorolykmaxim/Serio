package org.serio.desktop.platform;

import org.serio.core.clipboard.Clipboard;
import org.serio.core.notifications.Notifications;

import java.awt.*;
import java.awt.datatransfer.StringSelection;
import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * Responsible for interaction with the environment, in which the application is being executed.
 *
 * <p>This is the base class, that implements only the functionality, that has the same implementation
 * across all the different platforms. Platform-specific functionality should be implemented in it's inheritors.</p>
 */
public abstract class DesktopPlatform implements Notifications, Clipboard {
    protected final String applicationName;

    /**
     * Construct a platform.
     *
     * @param applicationName name of the application, that is being executed
     */
    public DesktopPlatform(String applicationName) {
        this.applicationName = applicationName;
    }

    /**
     * Get path on which the database file of the application may reside.
     *
     * @return path to the application's database
     */
    public Path getDatabasePath() {
        return Paths.get(System.getProperty("user.home"), ".serio.db");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setContent(String content) {
        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(content), null);
    }
}
