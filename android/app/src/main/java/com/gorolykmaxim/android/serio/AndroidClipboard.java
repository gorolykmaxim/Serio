package com.gorolykmaxim.android.serio;

import android.content.ClipData;
import android.content.ClipboardManager;

import org.serio.core.clipboard.Clipboard;

public class AndroidClipboard implements Clipboard {
    private final String applicationName;
    private final ClipboardManager clipboardManager;

    public AndroidClipboard(String applicationName, ClipboardManager clipboardManager) {
        this.applicationName = applicationName;
        this.clipboardManager = clipboardManager;
    }

    @Override
    public void setContent(String content) {
        ClipData clipData = ClipData.newPlainText(applicationName, content);
        clipboardManager.setPrimaryClip(clipData);
    }
}