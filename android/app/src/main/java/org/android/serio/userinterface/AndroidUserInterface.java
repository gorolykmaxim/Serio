package org.android.serio.userinterface;

import android.app.Activity;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import androidx.activity.ComponentActivity;
import androidx.lifecycle.MutableLiveData;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jdk8.Jdk8Module;
import org.android.serio.AndroidApplicationControllerDecorator;

import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.UserInterface;
import org.serio.core.userinterface.ViewIds;

public abstract class AndroidUserInterface implements UserInterface {
    private final String userInterfaceEntryPointLocation;
    private final ObjectMapper objectMapper;
    private final MutableLiveData<ApplicationEvent> applicationEventObserver;
    private final WebChromeClient chromeClient;
    private AndroidApplicationControllerDecorator applicationController;
    protected int currentView;

    public AndroidUserInterface(String userInterfaceEntryPointLocation) {
        this.userInterfaceEntryPointLocation = userInterfaceEntryPointLocation;
        objectMapper = new ObjectMapper();
        objectMapper.registerModule(new Jdk8Module());
        this.applicationEventObserver = new MutableLiveData<>();
        chromeClient = new DarkVideoPosterClient();
        currentView = -1;
    }

    public void setApplicationController(AndroidApplicationControllerDecorator applicationController) {
        this.applicationController = applicationController;
    }

    @Override
    public void sendEvent(ApplicationEvent event) {
        applicationEventObserver.postValue(event);
    }

    public void setWebView(WebView webView, ComponentActivity activity) {
        webView.setWebViewClient(new WebViewClient() {
            @Override
            public void onPageFinished(WebView view, String url) {
                super.onPageFinished(view, url);
                applicationEventObserver.observe(activity, event -> {
                    currentView = event.getViewId();
                    sendEventToWebView(event, webView, activity);
                });
            }
        });
        webView.setWebChromeClient(chromeClient);
        webView.addJavascriptInterface(applicationController, "serioController");
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
        webView.loadUrl(String.format("%s#platform=%d&runtimeType=%d", userInterfaceEntryPointLocation, getPlatform(), 0));
    }

    public void onActivityBackPressed(Runnable handleBack) {
        if (currentView == ViewIds.ALL_SHOWS) {
            handleBack.run();
        } else {
            applicationController.back();
        }
    }

    public abstract void onActivityCreate(Activity activity);

    public abstract void onActivityWindowFocusChanged(Activity activity, boolean hasFocus);

    protected void renderEvent(ApplicationEvent event, WebView webView) {
        try {
            String serializedEvent = objectMapper.writeValueAsString(event);;
            webView.loadUrl("javascript:dispatchApplicationEvent(" + serializedEvent + ")");
        } catch (JsonProcessingException e) {
            throw new RuntimeException(e);
        }
    }

    protected abstract void sendEventToWebView(ApplicationEvent event, WebView webView, ComponentActivity activity);

    protected abstract int getPlatform();
}
