package org.android.serio;

import android.content.Context;
import android.widget.Toast;

import androidx.lifecycle.LifecycleOwner;
import androidx.lifecycle.MutableLiveData;

import org.apache.commons.lang3.StringUtils;
import org.serio.core.notifications.Notifications;

/**
 * Android implementation of {@link Notifications}.
 *
 * <p>Displays notifications as {@link Toast} messages.</p>
 */
public class AndroidNotifications implements Notifications {
    private final MutableLiveData<String> notificationMessageObserver;

    /**
     * Construct notifications.
     */
    public AndroidNotifications() {
        this.notificationMessageObserver = new MutableLiveData<>();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void displayTextAsNotification(String text) {
        notificationMessageObserver.postValue(text);
    }

    /**
     * Start displaying all the notifications, that are being requested to be displayed,
     * in scope of the specified context.
     *
     * @param context context in scope of which the notifications will be displayed
     * @param lifecycleOwner owner of notifications lifecycle
     */
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
