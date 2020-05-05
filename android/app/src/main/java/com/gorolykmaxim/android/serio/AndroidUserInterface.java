package com.gorolykmaxim.android.serio;

import androidx.lifecycle.MutableLiveData;

import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.UserInterface;

public class AndroidUserInterface implements UserInterface {
    private MutableLiveData<ApplicationEvent> applicationEventObserver;

    public AndroidUserInterface(MutableLiveData<ApplicationEvent> applicationEventObserver) {
        this.applicationEventObserver = applicationEventObserver;
    }

    @Override
    public void sendEvent(ApplicationEvent event) {
        applicationEventObserver.postValue(event);
    }
}
