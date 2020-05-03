package org.serio.desktop.platform;

import org.apache.commons.lang3.SystemUtils;

public class Platforms {
    public static DesktopPlatform getCurrentPlatform(String applicationName) {
        if (SystemUtils.IS_OS_WINDOWS) {
            return new WindowsPlatform(applicationName);
        } else if (SystemUtils.IS_OS_MAC_OSX) {
            return new MacOsPlatform(applicationName);
        } else {
            return new GenericPlatform(applicationName);
        }
    }
}
