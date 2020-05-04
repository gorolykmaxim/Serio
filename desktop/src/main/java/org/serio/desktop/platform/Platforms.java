package org.serio.desktop.platform;

import org.apache.commons.lang3.SystemUtils;

/**
 * Factory of {@link DesktopPlatform} instances.
 */
public class Platforms {
    /**
     * Get the platform, on which the application is being currently executed on.
     *
     * @param applicationName name of the application
     * @return current desktop platform
     */
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
