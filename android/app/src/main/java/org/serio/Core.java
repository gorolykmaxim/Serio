package org.serio;

public class Core {
    static {
        System.loadLibrary("serio-android");
    }

    public native void sendEvent(String event);

    public void receiveEvent(String event) {
        System.out.println(event);
    }
}
