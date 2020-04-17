package org.serio.core.shows;

import org.serio.core.showstorage.ShowMetaData;
import org.serio.core.watchhistory.ShowView;

import java.time.LocalDate;
import java.util.Objects;
import java.util.Optional;
import java.util.UUID;

public class WatchableShowMetaData {
    private final ShowMetaData metaData;
    private final ShowView view;

    WatchableShowMetaData(ShowMetaData metaData, ShowView view) {
        this.metaData = metaData;
        this.view = view;
    }

    public UUID getId() {
        return metaData.getId();
    }

    public String getName() {
        return metaData.getName();
    }

    public String getThumbnailUrl() {
        return metaData.getThumbnailUrl();
    }

    public boolean hasBeenWatched() {
        return view != null;
    }

    public Optional<LocalDate> getLastWatchedDate() {
        return Optional.ofNullable(view != null ? view.getLastWatchedDate() : null);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        WatchableShowMetaData that = (WatchableShowMetaData) o;
        return Objects.equals(metaData, that.metaData) &&
                Objects.equals(view, that.view);
    }

    @Override
    public int hashCode() {
        return Objects.hash(metaData, view);
    }

    @Override
    public String toString() {
        return "WatchableShowMetaData{" +
                "metaData=" + metaData +
                ", view=" + view +
                '}';
    }
}
