package com.gorolykmaxim.android.serio;

import android.app.Application;
import android.content.ClipboardManager;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jdk8.Jdk8Module;
import com.gorolykmaxim.android.serio.storage.AndroidStorage;

import org.serio.core.Core;
import org.serio.core.clipboard.Clipboard;
import org.serio.core.httpclient.HttpClient;
import org.serio.core.notifications.Notifications;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.UserInterface;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class SerioApplication extends Application {
    private ObjectMapper mapper;
    private MutableLiveData<ApplicationEvent> applicationEventObserver;
    private MutableLiveData<String> notificationObserver;
    private AndroidApplicationControllerDecorator controller;

    @Override
    public void onCreate() {
        super.onCreate();
        mapper = new ObjectMapper();
        mapper.registerModule(new Jdk8Module());
        AndroidStorage storage = new AndroidStorage(this);
        applicationEventObserver = new MutableLiveData<>();
        notificationObserver = new MutableLiveData<>();
        UserInterface userInterface = new AndroidUserInterface(applicationEventObserver);
        Clipboard clipboard = new AndroidClipboard("Serio", (ClipboardManager) getSystemService(CLIPBOARD_SERVICE));
        Notifications notifications = new AndroidNotifications(notificationObserver);
        HttpClient httpClient = new AndroidHttpClient();
        Core core = new Core(clipboard, httpClient, notifications, storage, storage, storage, userInterface, storage);
        ExecutorService executorService = Executors.newSingleThreadExecutor();
        controller = new AndroidApplicationControllerDecorator(core.getApplicationController(), executorService);
        controller.viewAllShows();
    }

    public LiveData<ApplicationEvent> getApplicationEventObserver() {
        return applicationEventObserver;
    }

    public MutableLiveData<String> getNotificationObserver() {
        return notificationObserver;
    }

    public AndroidApplicationControllerDecorator getController() {
        return controller;
    }

    public String serializeEvent(ApplicationEvent event) {
        try {
            return mapper.writeValueAsString(event);
        } catch (JsonProcessingException e) {
            throw new RuntimeException(e);
        }
    }
}
