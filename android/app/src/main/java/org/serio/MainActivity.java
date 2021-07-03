package org.serio;

import android.graphics.Color;
import android.os.Bundle;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.LifecycleOwner;

public class MainActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        SerioApplication app = (SerioApplication) getApplication();
        setContentView(R.layout.activity_main);
        WebView webView = findViewById(R.id.rootWebView);
        webView.setBackgroundColor(Color.TRANSPARENT);
        webView.addJavascriptInterface(app.core, "core");
        LifecycleOwner lifecycleOwner = this;
        webView.setWebViewClient(new WebViewClient() {
            @Override
            public void onPageFinished(WebView view, String url) {
                super.onPageFinished(view, url);
                app.core.outgoing.observe(lifecycleOwner, (t) -> {
                    webView.loadUrl("javascript:displayView(" + t + ")");
                });
            }
        });
        WebSettings settings = webView.getSettings();
        settings.setJavaScriptEnabled(true);
        settings.setAllowFileAccess(true);
        settings.setAllowContentAccess(true);
        settings.setAllowFileAccessFromFileURLs(true);
        settings.setAllowUniversalAccessFromFileURLs(true);
        webView.loadUrl("file:///android_asset/index.html");
    }

    @Override
    public void onBackPressed() {
        SerioApplication app = (SerioApplication) getApplication();
        // If current view has "backTask" specified - send it to core.
        // If it does not - exit application.
        if (!app.core.sendBackTaskFromRenderTask(app.core.outgoing.getValue())) {
            super.onBackPressed();
        }
    }
}