package org.serio.core.applicationcontroller.model;

import org.serio.core.shows.WatchableShowMetaData;

import java.time.LocalDateTime;
import java.util.Objects;
import java.util.UUID;

public class DisplayableShowMetaData {
    private final WatchableShowMetaData metaData;
    private final DateFormat dateFormat;

    public DisplayableShowMetaData(WatchableShowMetaData metaData, DateFormat dateFormat) {
        if (metaData == null) {
            throw new IllegalArgumentException("Show meta-data is not specified");
        }
        if (dateFormat == null) {
            throw new IllegalArgumentException("Date format is not specified");
        }
        this.metaData = metaData;
        this.dateFormat = dateFormat;
    }

    public UUID getId() {
        return metaData.getId();
    }

    public String getName() {
        return metaData.getName();
    }

    public String getThumbnail() {
        return metaData.getThumbnailUrl();
    }

    public String getLastWatched() {
        return metaData.getLastWatchedDate()
                .map(LocalDateTime::toLocalDate)
                .map(dateFormat::applyTo)
                .orElse("");
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DisplayableShowMetaData that = (DisplayableShowMetaData) o;
        return Objects.equals(metaData, that.metaData) &&
                Objects.equals(dateFormat, that.dateFormat);
    }

    @Override
    public int hashCode() {
        return Objects.hash(metaData, dateFormat);
    }

    @Override
    public String toString() {
        return "DisplayableShow{" +
                "metaData=" + metaData +
                ", dateFormat=" + dateFormat +
                '}';
    }
}
