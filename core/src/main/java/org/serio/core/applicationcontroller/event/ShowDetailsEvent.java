package org.serio.core.applicationcontroller.event;

import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;

/**
 * Event that triggers rendering of {@link ViewIds#SHOW_DETAILS} view.
 */
public class ShowDetailsEvent implements ApplicationEvent {
    private final DisplayableShow show;

    /**
     * Construct an event.
     *
     * @param show show, details of which should be displayed. Can't be null.
     */
    public ShowDetailsEvent(DisplayableShow show) {
        if (show == null) {
            throw new IllegalArgumentException("Show is not specified");
        }
        this.show = show;
    }

    /**
     * Get show, information about which should be rendered.
     *
     * @return show to be displayed
     */
    public DisplayableShow getShow() {
        return show;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getViewId() {
        return ViewIds.SHOW_DETAILS;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ShowDetailsEvent that = (ShowDetailsEvent) o;
        return Objects.equals(show, that.show);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(show);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "ShowDetailsEvent{" +
                "show=" + show +
                '}';
    }
}
