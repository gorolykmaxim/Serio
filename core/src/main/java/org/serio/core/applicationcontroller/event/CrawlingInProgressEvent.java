package org.serio.core.applicationcontroller.event;

import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;

/**
 * Event that will trigger rendering of {@link ViewIds#CRAWLING_IN_PROGRESS} view.
 */
public class CrawlingInProgressEvent implements ApplicationEvent {

    /**
     * {@inheritDoc}
     */
    @Override
    public int getViewId() {
        return ViewIds.CRAWLING_IN_PROGRESS;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        return o != null && getClass() == o.getClass();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(getViewId());
    }
}
