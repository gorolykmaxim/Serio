package org.serio.desktop.platform;

import org.serio.core.clipboard.Clipboard;
import org.serio.core.notifications.Notifications;

import java.awt.*;
import java.awt.datatransfer.StringSelection;
import java.nio.file.Path;
import java.nio.file.Paths;

public abstract class DesktopPlatform implements Notifications, Clipboard {
    protected final String applicationName;

    public DesktopPlatform(String applicationName) {
        this.applicationName = applicationName;
    }

    public Path getDatabasePath() {
        return Paths.get(System.getProperty("user.home"), ".serio.db");
    }

    @Override
    public void setContent(String content) {
        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(content), null);
    }
}
