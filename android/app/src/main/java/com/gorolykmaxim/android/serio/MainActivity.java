package com.gorolykmaxim.android.serio;

import android.app.UiModeManager;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.os.Bundle;
import android.view.View;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import org.serio.core.userinterface.ViewIds;

public class MainActivity extends AppCompatActivity {
    private static final String IS_DISPLAYING_FULL_SCREEN_VIEW_KEY = "IS_DISPLAYING_FULL_SCREEN_VIEW_KEY";
    private static final String IS_DISPLAYING_ROOT_VIEW_KEY = "IS_DISPLAYING_ROOT_VIEW_KEY";
    private boolean isDisplayingFullScreenView;
    private boolean isDisplayingRootViewKey;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        isDisplayingFullScreenView = savedInstanceState != null && savedInstanceState.getBoolean(IS_DISPLAYING_FULL_SCREEN_VIEW_KEY, false);
        isDisplayingRootViewKey = savedInstanceState != null && savedInstanceState.getBoolean(IS_DISPLAYING_ROOT_VIEW_KEY, false);
        if (!isTv()) {
            setRequestedOrientation(isDisplayingFullScreenView ? ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE : ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        }
        setUpSystemVisibility();
        setContentView(R.layout.activity_main);
        WebView webView = findViewById(R.id.web_view);
        SerioApplication application = (SerioApplication) getApplication();
        application.getNotificationObserver().observe(this, notification -> {
            Toast.makeText(this, notification, Toast.LENGTH_SHORT).show();
        });
        MainActivity activity = this;
        webView.setWebViewClient(new WebViewClient() {
            @Override
            public void onPageFinished(WebView view, String url) {
                super.onPageFinished(view, url);
                application.getApplicationEventObserver().observe(activity, event -> {
                    isDisplayingRootViewKey = event.getViewId() == ViewIds.ALL_SHOWS;
                    boolean previousState = isDisplayingFullScreenView;
                    isDisplayingFullScreenView = event.getViewId() == ViewIds.SHOW_PLAYER;
                    if (!isTv() && previousState != isDisplayingFullScreenView) {
                        activity.recreate();
                    } else {
                        webView.loadUrl("javascript:dispatchApplicationEvent(" + application.serializeEvent(event) + ")");
                    }
                });
            }
        });
        webView.setWebChromeClient(new WebChromeClient() {
            @Nullable
            @Override
            public Bitmap getDefaultVideoPoster() {
                final Bitmap bitmap = Bitmap.createBitmap(1, 1, Bitmap.Config.RGB_565);
                Canvas canvas = new Canvas(bitmap);
                canvas.drawARGB(255, 0, 0, 0);
                return bitmap;
            }
        });
        webView.addJavascriptInterface(application.getController(), "serioController");
        WebSettings settings = webView.getSettings();
        settings.setJavaScriptEnabled(true);
        settings.setMediaPlaybackRequiresUserGesture(false);
        settings.setAllowFileAccess(true);
        settings.setLoadsImagesAutomatically(true);
        settings.setAppCacheEnabled(true);
        settings.setDomStorageEnabled(true);
        settings.setDatabaseEnabled(true);
        settings.setAllowContentAccess(true);
        settings.setAllowFileAccessFromFileURLs(true);
        settings.setAllowUniversalAccessFromFileURLs(true);
        webView.loadUrl(String.format("file:///android_asset/index.html#platform=%d&runtimeType=%d", isTv() ? 2 : 1, 0));
    }

    @Override
    protected void onSaveInstanceState(@NonNull Bundle outState) {
        outState.putBoolean(IS_DISPLAYING_FULL_SCREEN_VIEW_KEY, isDisplayingFullScreenView);
        outState.putBoolean(IS_DISPLAYING_ROOT_VIEW_KEY, isDisplayingRootViewKey);
        super.onSaveInstanceState(outState);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        if (hasFocus) {
            setUpSystemVisibility();
        }
    }

    @Override
    public void onBackPressed() {
        if (isDisplayingRootViewKey) {
            super.onBackPressed();
        } else {
            SerioApplication application = (SerioApplication) getApplication();
            application.getController().back();
        }
    }

    private void setUpSystemVisibility() {
        if (!isTv() && isDisplayingFullScreenView) {
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_FULLSCREEN);
        }
    }

    private boolean isTv() {
        UiModeManager uiModeManager = (UiModeManager) getSystemService(UI_MODE_SERVICE);
        return uiModeManager.getCurrentModeType() == Configuration.UI_MODE_TYPE_TELEVISION;
    }
}
