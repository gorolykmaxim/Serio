package com.gorolykmaxim.android.serio.userinterface;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.view.View;
import android.webkit.WebView;

import androidx.activity.ComponentActivity;

import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class AndroidMobileUserInterface extends AndroidUserInterface {
    private static final Set<Integer> FULL_SCREEN_VIEWS = Stream.of(ViewIds.SHOW_PLAYER).collect(Collectors.toSet());
    private int lastView;

    public AndroidMobileUserInterface(String userInterfaceEntryPointLocation) {
        super(userInterfaceEntryPointLocation);
        lastView = -1;
    }

    @Override
    public void onActivityCreate(Activity activity) {
        activity.setRequestedOrientation(isDisplayingFullScreenView() ? ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE : ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        if (isDisplayingFullScreenView()) {
            displayActivityInFullScreen(activity);
        }
    }

    @Override
    public void onActivityWindowFocusChanged(Activity activity, boolean hasFocus) {
        if (hasFocus && isDisplayingFullScreenView()) {
            displayActivityInFullScreen(activity);
        }
    }

    @Override
    protected void sendEventToWebView(ApplicationEvent event, WebView webView, ComponentActivity activity) {
        int previousView = lastView;
        lastView = currentView;
        if (currentView != previousView && (FULL_SCREEN_VIEWS.contains(currentView) || FULL_SCREEN_VIEWS.contains(previousView))) {
            activity.recreate();
        } else {
            renderEvent(event, webView);
        }
    }

    @Override
    protected int getPlatform() {
        return 1;
    }

    private boolean isDisplayingFullScreenView() {
        return FULL_SCREEN_VIEWS.contains(currentView);
    }

    private void displayActivityInFullScreen(Activity activity) {
        activity.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_FULLSCREEN);
    }
}
