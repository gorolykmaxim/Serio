package org.serio.core.applicationcontroller.model;

import org.serio.core.shows.WatchableShowMetaData;

import java.time.LocalDateTime;
import java.util.Objects;
import java.util.UUID;

/**
 * A TV show metadata that contains all the necessary information to be displayed in the user interface.
 *
 * <p>Serves as an interface adapter between a {@link WatchableShowMetaData} and the representation, that is
 * understandable by the user interface.</p>
 */
public class DisplayableShowMetaData {
    private final WatchableShowMetaData metaData;
    private final DateFormat dateFormat;

    /**
     * Construct a displayable show metadata based off of the specified metadata.
     *
     * @param metaData metadata to be based off of
     * @param dateFormat date format to apply to the show's last watched date
     */
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

    /**
     * @see WatchableShowMetaData#getId()
     */
    public UUID getId() {
        return metaData.getId();
    }

    /**
     * @see WatchableShowMetaData#getName()
     */
    public String getName() {
        return metaData.getName();
    }

    /**
     * @see WatchableShowMetaData#getThumbnailUrl()
     */
    public String getThumbnail() {
        return metaData.getThumbnailUrl();
    }

    /**
     * Get the date when the show has been watched the last time, while applying a corresponding {@link DateFormat}.
     *
     * @see WatchableShowMetaData#getLastWatchedDate()
     *
     * @return formatted show's last watched date or an empty string if the show has not been watched yet
     */
    public String getLastWatched() {
        return metaData.getLastWatchedDate()
                .map(LocalDateTime::toLocalDate)
                .map(dateFormat::applyTo)
                .orElse("");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DisplayableShowMetaData that = (DisplayableShowMetaData) o;
        return Objects.equals(metaData, that.metaData) &&
                Objects.equals(dateFormat, that.dateFormat);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(metaData, dateFormat);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "DisplayableShow{" +
                "metaData=" + metaData +
                ", dateFormat=" + dateFormat +
                '}';
    }
}
