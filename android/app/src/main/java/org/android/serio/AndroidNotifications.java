package org.android.serio;

import android.content.Context;
import android.widget.Toast;

import androidx.lifecycle.LifecycleOwner;
import androidx.lifecycle.MutableLiveData;

import org.apache.commons.lang3.StringUtils;
import org.serio.core.notifications.Notifications;

public class AndroidNotifications implements Notifications {
    private final MutableLiveData<String> notificationMessageObserver;

    public AndroidNotifications() {
        this.notificationMessageObserver = new MutableLiveData<>();
    }

    @Override
    public void displayTextAsNotification(String text) {
        notificationMessageObserver.postValue(text);
    }

    public void displayNotificationsInScopeOf(Context context, LifecycleOwner lifecycleOwner) {
        notificationMessageObserver.observe(lifecycleOwner, notification -> {
            if (StringUtils.isNotEmpty(notification)) {
                Toast.makeText(context, notification, Toast.LENGTH_SHORT).show();
                // Set current value to an empty string to indicate that the last notification
                // has been displayed.
                notificationMessageObserver.setValue("");
            }
        });
    }
}
