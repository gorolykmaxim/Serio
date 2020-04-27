package org.serio.core.applicationcontroller.event;

import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;

public class CrawlingInProgressEvent implements ApplicationEvent {
    @Override
    public int getViewId() {
        return ViewIds.CRAWLING_IN_PROGRESS;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        return o != null && getClass() == o.getClass();
    }

    @Override
    public int hashCode() {
        return Objects.hash(getViewId());
    }
}
