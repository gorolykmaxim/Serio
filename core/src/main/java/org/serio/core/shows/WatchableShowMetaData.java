package org.serio.core.shows;

import org.serio.core.showstorage.ShowMetaData;
import org.serio.core.watchhistory.ShowView;

import java.time.LocalDate;
import java.util.Objects;
import java.util.Optional;
import java.util.UUID;

/**
 * Meta-data of a show that the user might be watching or might have watched.
 *
 * @see ShowMetaData
 */
public class WatchableShowMetaData {
    private final ShowMetaData metaData;
    private final ShowView view;

    /**
     * Create {@link WatchableShowMetaData} representation of the specified {@link ShowMetaData} taking into
     * consideration the specified show view.
     *
     * @param metaData actual meta-data of the show
     * @param view view status of the specified show
     */
    public WatchableShowMetaData(ShowMetaData metaData, ShowView view) {
        if (metaData == null) {
            throw new IllegalArgumentException("metaData is not specified");
        }
        this.metaData = metaData;
        this.view = view;
    }

    /**
     * @see ShowMetaData#getId()
     */
    public UUID getId() {
        return metaData.getId();
    }

    /**
     * @see ShowMetaData#getName()
     */
    public String getName() {
        return metaData.getName();
    }

    /**
     * @see ShowMetaData#getThumbnailUrl()
     */
    public String getThumbnailUrl() {
        return metaData.getThumbnailUrl();
    }

    /**
     * Check if this show has been watched by the user.
     *
     * @return true if the show has been watched by the user
     */
    public boolean hasBeenWatched() {
        return view != null;
    }

    /**
     * Get the last date the user has watched one of the episodes of the show
     *
     * @see ShowView#getLastWatchedDate()
     *
     * @return the last date the user has watched the show
     */
    public Optional<LocalDate> getLastWatchedDate() {
        return Optional.ofNullable(view != null ? view.getLastWatchedDate() : null);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        WatchableShowMetaData that = (WatchableShowMetaData) o;
        return Objects.equals(metaData, that.metaData) &&
                Objects.equals(view, that.view);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(metaData, view);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "WatchableShowMetaData{" +
                "metaData=" + metaData +
                ", view=" + view +
                '}';
    }
}
