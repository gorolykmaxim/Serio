package com.gorolykmaxim.android.serio.userinterface;

import android.app.Activity;
import android.webkit.WebView;

import androidx.activity.ComponentActivity;

import org.serio.core.userinterface.ApplicationEvent;

public class AndroidTvUserInterface extends AndroidUserInterface {
    public AndroidTvUserInterface(String userInterfaceEntryPointLocation) {
        super(userInterfaceEntryPointLocation);
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
