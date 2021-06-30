package org.serio;

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

    public native void sendEvent(String event);

    public void receiveEvent(String event) {
        outgoing.postValue(event);
    }
}
