package org.android.serio.userinterface;

import android.app.UiModeManager;
import android.content.Context;
import android.content.res.Configuration;

import java.util.concurrent.ScheduledExecutorService;

public class AndroidUserInterfaces {
    public static AndroidUserInterface createForCurrentPlatform(String userInterfaceEntryPointLocation, Context context, ScheduledExecutorService scheduledExecutorService) {
        UiModeManager modeManager = (UiModeManager) context.getSystemService(Context.UI_MODE_SERVICE);
        if (modeManager.getCurrentModeType() == Configuration.UI_MODE_TYPE_TELEVISION) {
            return new AndroidTvUserInterface(userInterfaceEntryPointLocation, scheduledExecutorService);
        } else {
            return new AndroidMobileUserInterface(userInterfaceEntryPointLocation, scheduledExecutorService);
        }
    }
}
