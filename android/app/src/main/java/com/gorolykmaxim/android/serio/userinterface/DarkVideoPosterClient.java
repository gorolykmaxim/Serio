package com.gorolykmaxim.android.serio.userinterface;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.webkit.WebChromeClient;

import androidx.annotation.Nullable;

public class DarkVideoPosterClient extends WebChromeClient {
    private final Bitmap defaultVideoPoster;

    public DarkVideoPosterClient() {
        defaultVideoPoster = Bitmap.createBitmap(1, 1, Bitmap.Config.RGB_565);
        Canvas canvas = new Canvas(defaultVideoPoster);
        canvas.drawARGB(255, 0, 0, 0);
    }

    @Nullable
    @Override
    public Bitmap getDefaultVideoPoster() {
        return defaultVideoPoster;
    }
}
