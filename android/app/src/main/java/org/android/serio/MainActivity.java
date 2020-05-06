package org.android.serio;

import android.os.Bundle;
import android.webkit.WebView;

import androidx.appcompat.app.AppCompatActivity;

import org.android.serio.userinterface.AndroidUserInterface;

/**
 * Main activity of the application, which only has a {@link WebView} in it, where the actual
 * user interface is being rendered in.
 */
public class MainActivity extends AppCompatActivity {
    private AndroidUserInterface userInterface;

    /**
     * {@inheritDoc}
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        SerioApplication application = (SerioApplication) getApplication();
        application.getNotifications().displayNotificationsInScopeOf(this,this);
        userInterface = application.getUserInterface();
        userInterface.onActivityCreate(this);
        setContentView(R.layout.activity_main);
        WebView webView = findViewById(R.id.web_view);
        userInterface.setWebView(webView, this);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        userInterface.onActivityWindowFocusChanged(this, hasFocus);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void onBackPressed() {
        userInterface.onActivityBackPressed(super::onBackPressed);
    }
}
