package com.gorolykmaxim.android.serio;

import androidx.lifecycle.MutableLiveData;

import org.serio.core.notifications.Notifications;

public class AndroidNotifications implements Notifications {
    private final MutableLiveData<String> notificationMessageObserver;

    public AndroidNotifications(MutableLiveData<String> notificationMessageObserver) {
        this.notificationMessageObserver = notificationMessageObserver;
    }

    @Override
    public void displayTextAsNotification(String text) {
        notificationMessageObserver.postValue(text);
    }
}
