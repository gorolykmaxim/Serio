package org.android.serio;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;

import org.serio.core.clipboard.Clipboard;

public class AndroidClipboard implements Clipboard {
    private final String applicationName;
    private final ClipboardManager clipboardManager;

    public AndroidClipboard(String applicationName, Context context) {
        this.applicationName = applicationName;
        this.clipboardManager = (ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);
    }

    @Override
    public void setContent(String content) {
        ClipData clipData = ClipData.newPlainText(applicationName, content);
        clipboardManager.setPrimaryClip(clipData);
    }
}
