package org.serio;

import android.webkit.JavascriptInterface;

import androidx.lifecycle.MutableLiveData;

public class Core extends Thread {
    static {
        System.loadLibrary("serio-android");
    }
    public final MutableLiveData<String> outgoing;
    private final String databasePath;

    public Core(String databasePath) {
        outgoing = new MutableLiveData<>();
        this.databasePath = databasePath;
    }

    @Override
    public void run() {
        runNative(databasePath);
    }

    public native void runNative(String databasePath);

    public native boolean sendBackTaskFromRenderTask(String renderTask);

    @JavascriptInterface
    public native void sendTask(String task);

    public void receiveRenderTask(String task) {
        outgoing.postValue(task);
    }
}
