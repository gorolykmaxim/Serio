package org.serio.desktop.platform;

import java.awt.*;
import java.net.URL;

public class WindowsPlatform extends DesktopPlatform {
    public WindowsPlatform(String applicationName) {
        super(applicationName);
    }

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
