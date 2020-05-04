package org.serio.desktop.platform;

import java.io.IOException;

/**
 * Mac OS {@link DesktopPlatform}.
 */
public class MacOsPlatform extends DesktopPlatform {
    /**
     * @see DesktopPlatform#DesktopPlatform(String)
     */
    public MacOsPlatform(String applicationName) {
        super(applicationName);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void displayTextAsNotification(String text) {
        try {
            Runtime.getRuntime().exec(new String[] {"osascript", "-e", String.format("display notification \"%s\" with title \"%s\"", text, applicationName)});
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
