package org.android.serio.userinterface;

import android.app.UiModeManager;
import android.content.Context;
import android.content.res.Configuration;

public class AndroidUserInterfaces {
    public static AndroidUserInterface createForCurrentPlatform(String userInterfaceEntryPointLocation, Context context) {
        UiModeManager modeManager = (UiModeManager) context.getSystemService(Context.UI_MODE_SERVICE);
        if (modeManager.getCurrentModeType() == Configuration.UI_MODE_TYPE_TELEVISION) {
            return new AndroidTvUserInterface(userInterfaceEntryPointLocation);
        } else {
            return new AndroidMobileUserInterface(userInterfaceEntryPointLocation);
        }
    }
}
