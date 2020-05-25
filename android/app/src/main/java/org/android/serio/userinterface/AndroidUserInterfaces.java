package org.android.serio.userinterface;

import android.app.UiModeManager;
import android.content.Context;
import android.content.res.Configuration;

/**
 * Factory of {@link AndroidUserInterface}.
 */
public class AndroidUserInterfaces {
    /**
     * Construct a {@link AndroidUserInterface} for the platform, on which the application
     * currently runs on.
     *
     * @param userInterfaceEntryPointLocation absolute path to index.html, from which the actual
     *                                        user interface assets can be accessed
     * @param context context, that will be used to determine current platform
     * @return user interface for the current platform
     */
    public static AndroidUserInterface createForCurrentPlatform(String userInterfaceEntryPointLocation, Context context) {
        UiModeManager modeManager = (UiModeManager) context.getSystemService(Context.UI_MODE_SERVICE);
        if (modeManager.getCurrentModeType() == Configuration.UI_MODE_TYPE_TELEVISION) {
            return new AndroidTvUserInterface(userInterfaceEntryPointLocation);
        } else {
            return new AndroidMobileUserInterface(userInterfaceEntryPointLocation);
        }
    }
}
