package com.gorolykmaxim.android.serio;

import android.app.Application;

import com.gorolykmaxim.android.serio.storage.AndroidStorage;
import com.gorolykmaxim.android.serio.userinterface.AndroidUserInterface;
import com.gorolykmaxim.android.serio.userinterface.AndroidUserInterfaces;

import org.serio.core.Core;
import org.serio.core.applicationcontroller.BackgroundThreadApplicationControllerProxy;
import org.serio.core.clipboard.Clipboard;
import org.serio.core.httpclient.HttpClient;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class SerioApplication extends Application {
    private AndroidNotifications notifications;
    private AndroidUserInterface userInterface;

    @Override
    public void onCreate() {
        super.onCreate();
        AndroidStorage storage = new AndroidStorage(this);
        userInterface = AndroidUserInterfaces.createForCurrentPlatform("file:///android_asset/index.html", this);
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
        androidController.viewAllShows();
    }

    public AndroidNotifications getNotifications() {
        return notifications;
    }

    public AndroidUserInterface getUserInterface() {
        return userInterface;
    }
}
