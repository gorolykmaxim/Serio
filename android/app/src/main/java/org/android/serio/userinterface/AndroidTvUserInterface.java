package org.android.serio.userinterface;

import android.app.Activity;
import android.webkit.WebView;

import androidx.activity.ComponentActivity;

import org.serio.core.userinterface.ApplicationEvent;

/**
 * {@link AndroidUserInterface} on TV devices.
 */
public class AndroidTvUserInterface extends AndroidUserInterface {
    /**
     * @see AndroidUserInterface#AndroidUserInterface(String)
     */
    public AndroidTvUserInterface(String userInterfaceEntryPointLocation) {
        super(userInterfaceEntryPointLocation);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void onActivityCreate(Activity activity) {

    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void onActivityWindowFocusChanged(Activity activity, boolean hasFocus) {

    }

    /**
     * {@inheritDoc}
     */
    @Override
    protected void sendEventToWebView(ApplicationEvent event, WebView webView, ComponentActivity activity) {
        renderEvent(event, webView);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    protected int getPlatform() {
        return 2;
    }
}
