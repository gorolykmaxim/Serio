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
import org.serio.core.applicationcontroller.ApplicationController;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.UserInterface;
import org.serio.core.userinterface.ViewIds;

import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * Android implementation of {@link UserInterface}.
 *
 * <p>Wires-up {@link ApplicationController} and {@link WebView}
 * together, while intersecting every {@link ApplicationEvent}, being passed through, to improve
 * the user experience (toggling the fullscreen for the corresponding views for example).</p>
 *
 * <p>It will trigger {@link ApplicationController#viewAllShows()} as soon as the user interface
 * assets complete loading in the {@link WebView}. It will also fully configure the latter
 * one to make sure that the UI assets will be properly displayed in it.</p>
 *
 * <p>It will also make sure, that the application's logo will be displayed for
 * {@link AndroidUserInterface#LOGO_DISPLAY_TIME_IN_MILLISECONDS}. It is done to prevent a flicker
 * of a logo: without a proper delay the logo will appear on a screen for a very-very short
 * period of time and then the {@link ViewIds#ALL_SHOWS} will be displayed. The interval of
 * time in such case will be short enough to be seem like a weird bug.</p>
 */
public abstract class AndroidUserInterface implements UserInterface {
    public static final long LOGO_DISPLAY_TIME_IN_MILLISECONDS = 1500;
    private final String userInterfaceEntryPointLocation;
    private final ObjectMapper objectMapper;
    private final MutableLiveData<ApplicationEvent> applicationEventObserver;
    private final WebChromeClient chromeClient;
    private final ScheduledExecutorService scheduledExecutorService;
    private AndroidApplicationControllerDecorator applicationController;
    private boolean isInitialized;
    protected int currentView;

    /**
     * Construct a user interface.
     *
     * @param userInterfaceEntryPointLocation absolute path to the entry point of the UI
     *                                        assets - index.html
     * @param scheduledExecutorService scheduled executor service, that will be used to bootstrap
     *                                 the {@link ApplicationController} by calling
     *                                 {@link ApplicationController#viewAllShows()} in it after
     *                                 a {@link AndroidUserInterface#LOGO_DISPLAY_TIME_IN_MILLISECONDS}.
     */
    public AndroidUserInterface(String userInterfaceEntryPointLocation, ScheduledExecutorService scheduledExecutorService) {
        this.userInterfaceEntryPointLocation = userInterfaceEntryPointLocation;
        this.scheduledExecutorService = scheduledExecutorService;
        objectMapper = new ObjectMapper();
        objectMapper.registerModule(new Jdk8Module());
        this.applicationEventObserver = new MutableLiveData<>();
        chromeClient = new DarkVideoPosterClient();
        currentView = -1;
        isInitialized = false;
    }

    /**
     * Set the application controller to wire-up with {@link WebView}.
     *
     * @param applicationController application controller to wire
     */
    public void setApplicationController(AndroidApplicationControllerDecorator applicationController) {
        this.applicationController = applicationController;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void sendEvent(ApplicationEvent event) {
        applicationEventObserver.postValue(event);
    }

    /**
     * Configure the specified web view, wire it with the corresponding application controller and
     * display the user interface assets in it.
     *
     * @param webView web view to render user interface in
     * @param activity activity in scope of which the web view is being rendered. The user interface
     *                 will interact with it in case it would need to perform some operations
     *                 to improve UX for a currently displayed view
     */
    public void setWebView(WebView webView, ComponentActivity activity) {
        webView.setWebViewClient(new WebViewClient() {
            @Override
            public void onPageFinished(WebView view, String url) {
                super.onPageFinished(view, url);
                if (!isInitialized) {
                    isInitialized = true;
                    scheduledExecutorService.schedule(applicationController::viewAllShows, LOGO_DISPLAY_TIME_IN_MILLISECONDS, TimeUnit.MILLISECONDS);
                }
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

    /**
     * Process the "back" button press.
     *
     * @param handleBack callback, calling which will proceed the normal "back" press android
     *                   handling flow
     */
    public void onActivityBackPressed(Runnable handleBack) {
        if (currentView == ViewIds.ALL_SHOWS) {
            handleBack.run();
        } else {
            applicationController.back();
        }
    }

    /**
     * Configure the specified activity, when it is being created.
     *
     * @param activity activity to configure
     */
    public abstract void onActivityCreate(Activity activity);

    /**
     * Configure the specified activity, when the window, displaying it, will get or loose
     * a focus.
     *
     * @param activity activity to configure
     * @param hasFocus true if the window got focus. false otherwise.
     */
    public abstract void onActivityWindowFocusChanged(Activity activity, boolean hasFocus);

    /**
     * Render the specified event in the user interface in the specified web view.
     *
     * @param event event to render
     * @param webView web view to render the event in
     */
    protected void renderEvent(ApplicationEvent event, WebView webView) {
        try {
            String serializedEvent = objectMapper.writeValueAsString(event);;
            webView.loadUrl("javascript:dispatchApplicationEvent(" + serializedEvent + ")");
        } catch (JsonProcessingException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Send the specified event for rendering in the specified web view.
     *
     * @param event event, that will be rendered
     * @param webView web view, that will render the event
     * @param activity activity, that houses the web view and may be modified during the rendering
     *                 to improve the UX
     */
    protected abstract void sendEventToWebView(ApplicationEvent event, WebView webView, ComponentActivity activity);

    /**
     * Get ID of the platform, for which this user interface was created for.
     *
     * @return ID of the current platform
     */
    protected abstract int getPlatform();
}
