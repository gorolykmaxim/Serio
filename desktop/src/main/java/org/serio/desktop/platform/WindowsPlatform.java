package org.serio.desktop.platform;

import java.awt.*;
import java.net.URL;

/**
 * Windows {@link DesktopPlatform}.
 */
public class WindowsPlatform extends DesktopPlatform {
    /**
     * @see DesktopPlatform#DesktopPlatform(String)
     */
    public WindowsPlatform(String applicationName) {
        super(applicationName);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void displayTextAsNotification(String text) {
        try {
            if (SystemTray.isSupported()) {
                SystemTray tray = SystemTray.getSystemTray();
                Image trayImage = Toolkit.getDefaultToolkit().createImage(new URL(""));
                TrayIcon trayIcon = new TrayIcon(trayImage, applicationName);
                trayIcon.setImageAutoSize(true);
                tray.add(trayIcon);
                trayIcon.displayMessage(applicationName, text, TrayIcon.MessageType.INFO);
                tray.remove(trayIcon);
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}
