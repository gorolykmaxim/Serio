package org.android.serio.userinterface;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.webkit.WebChromeClient;

import androidx.annotation.Nullable;

/**
 * {@link WebChromeClient} that displays a black screen as a placeholder for <video/> while the
 * latter one loads.
 */
public class DarkVideoPosterClient extends WebChromeClient {
    private final Bitmap defaultVideoPoster;

    /**
     * Construct a chrome client.
     */
    public DarkVideoPosterClient() {
        defaultVideoPoster = Bitmap.createBitmap(1, 1, Bitmap.Config.RGB_565);
        Canvas canvas = new Canvas(defaultVideoPoster);
        canvas.drawARGB(255, 0, 0, 0);
    }

    /**
     * {@inheritDoc}
     */
    @Nullable
    @Override
    public Bitmap getDefaultVideoPoster() {
        return defaultVideoPoster;
    }
}
