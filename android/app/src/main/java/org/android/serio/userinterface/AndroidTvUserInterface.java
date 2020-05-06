package org.android.serio.userinterface;

import android.app.Activity;
import android.webkit.WebView;

import androidx.activity.ComponentActivity;

import org.serio.core.userinterface.ApplicationEvent;

import java.util.concurrent.ScheduledExecutorService;

public class AndroidTvUserInterface extends AndroidUserInterface {

    public AndroidTvUserInterface(String userInterfaceEntryPointLocation, ScheduledExecutorService scheduledExecutorService) {
        super(userInterfaceEntryPointLocation, scheduledExecutorService);
    }

    @Override
    public void onActivityCreate(Activity activity) {

    }

    @Override
    public void onActivityWindowFocusChanged(Activity activity, boolean hasFocus) {

    }

    @Override
    protected void sendEventToWebView(ApplicationEvent event, WebView webView, ComponentActivity activity) {
        renderEvent(event, webView);
    }

    @Override
    protected int getPlatform() {
        return 2;
    }
}
