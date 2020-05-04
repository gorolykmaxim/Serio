package org.serio.desktop.platform;

/**
 * A {@link DesktopPlatform}, that represents any unknown platform (currently it represents linux).
 *
 * <p>This is a NULL-object {@link DesktopPlatform}, thus the functionality, that is not implemented in
 * the {@link DesktopPlatform}, is not implemented in here either and is left stubbed.</p>
 */
public class GenericPlatform extends DesktopPlatform {
    /**
     * @see DesktopPlatform#DesktopPlatform(String)
     */
    public GenericPlatform(String applicationName) {
        super(applicationName);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void displayTextAsNotification(String text) {

    }
}
