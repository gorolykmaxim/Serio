package org.serio;

import androidx.lifecycle.MutableLiveData;

public class Core extends Thread {
    static {
        System.loadLibrary("serio-android");
    }
    public final MutableLiveData<String> outgoing = new MutableLiveData<>();

    @Override
    public void run() {
        runNative();
    }

    public native void runNative();

    public native void sendEvent(String event);

    public void receiveEvent(String event) {
        outgoing.postValue(event);
    }
}
