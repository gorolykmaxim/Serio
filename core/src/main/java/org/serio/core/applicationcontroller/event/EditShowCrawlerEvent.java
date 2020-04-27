package org.serio.core.applicationcontroller.event;

import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;
import java.util.Optional;
import java.util.UUID;

public class EditShowCrawlerEvent implements ApplicationEvent {
    private final UUID showId;
    private final String showName;

    public EditShowCrawlerEvent(UUID showId, String showName) {
        this.showId = showId;
        this.showName = showName;
    }

    public Optional<UUID> getShowId() {
        return Optional.ofNullable(showId);
    }

    public Optional<String> getShowName() {
        return Optional.ofNullable(showName);
    }

    @Override
    public int getViewId() {
        return ViewIds.EDIT_SHOW_CRAWLER;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        EditShowCrawlerEvent that = (EditShowCrawlerEvent) o;
        return Objects.equals(showId, that.showId) &&
                Objects.equals(showName, that.showName);
    }

    @Override
    public int hashCode() {
        return Objects.hash(showId, showName);
    }

    @Override
    public String toString() {
        return "EditShowCrawlerEvent{" +
                "showId=" + showId +
                ", showName='" + showName + '\'' +
                '}';
    }
}
