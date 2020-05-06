package org.android.serio;

import android.app.Application;

import org.android.serio.storage.AndroidStorage;
import org.android.serio.userinterface.AndroidUserInterface;
import org.android.serio.userinterface.AndroidUserInterfaces;
import org.serio.core.Core;
import org.serio.core.applicationcontroller.BackgroundThreadApplicationControllerProxy;
import org.serio.core.clipboard.Clipboard;
import org.serio.core.httpclient.HttpClient;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;

/**
 * Entry point of the application.
 *
 * <p>Constructs all the modules of the application and wires them together.</p>
 */
public class SerioApplication extends Application {
    private AndroidNotifications notifications;
    private AndroidUserInterface userInterface;

    /**
     * {@inheritDoc}
     */
    @Override
    public void onCreate() {
        super.onCreate();
        AndroidStorage storage = new AndroidStorage(this);
        ScheduledExecutorService scheduledExecutorService = Executors.newSingleThreadScheduledExecutor();
        userInterface = AndroidUserInterfaces.createForCurrentPlatform("file:///android_asset/index.html", this, scheduledExecutorService);
        Clipboard clipboard = new AndroidClipboard("Serio", this);
        notifications = new AndroidNotifications();
        HttpClient httpClient = new AndroidHttpClient();
        Core core = new Core(clipboard, httpClient, notifications, storage, storage, storage, userInterface, storage);
        ExecutorService executorService = Executors.newSingleThreadExecutor();
        AndroidApplicationControllerDecorator androidController = new AndroidApplicationControllerDecorator(
                new BackgroundThreadApplicationControllerProxy(
                        core.getApplicationController(),
                        executorService
                )
        );
        userInterface.setApplicationController(androidController);
    }

    /**
     * Get the module, used to display notifications
     *
     * @return notifications modules
     */
    public AndroidNotifications getNotifications() {
        return notifications;
    }

    /**
     * Get the module, used to display the user interface.
     *
     * @return user interface module
     */
    public AndroidUserInterface getUserInterface() {
        return userInterface;
    }
}
