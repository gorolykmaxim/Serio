package org.serio.core.applicationcontroller.event;

import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;

public class ShowDetailsEvent implements ApplicationEvent {
    private final DisplayableShow show;

    public ShowDetailsEvent(DisplayableShow show) {
        if (show == null) {
            throw new IllegalArgumentException("Show is not specified");
        }
        this.show = show;
    }

    public DisplayableShow getShow() {
        return show;
    }

    @Override
    public int getViewId() {
        return ViewIds.SHOW_DETAILS;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ShowDetailsEvent that = (ShowDetailsEvent) o;
        return Objects.equals(show, that.show);
    }

    @Override
    public int hashCode() {
        return Objects.hash(show);
    }

    @Override
    public String toString() {
        return "ShowDetailsEvent{" +
                "show=" + show +
                '}';
    }
}
