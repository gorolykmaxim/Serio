package org.android.serio;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;

import org.serio.core.clipboard.Clipboard;

/**
 * Android implementation of the {@link Clipboard}.
 */
public class AndroidClipboard implements Clipboard {
    private final String applicationName;
    private final ClipboardManager clipboardManager;

    /**
     * Construct a clipboard.
     *
     * @param applicationName name of the application that will be used to separate clipboard
     *                        entries of this application from the other ones.
     * @param context context that will be used to access the clipboard service
     */
    public AndroidClipboard(String applicationName, Context context) {
        this.applicationName = applicationName;
        this.clipboardManager = (ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setContent(String content) {
        ClipData clipData = ClipData.newPlainText(applicationName, content);
        clipboardManager.setPrimaryClip(clipData);
    }
}
