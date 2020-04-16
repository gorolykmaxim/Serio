package org.serio.core.watchhistory;

import java.time.LocalDate;
import java.util.Objects;

public class ShowView {
    private final String showId;
    private final LocalDate lastWatchedDate;

    public ShowView(String showId) {
        this(showId, null);
    }

    public ShowView(String showId, LocalDate lastWatchedDate) {
        if (showId == null) {
            throw new IllegalArgumentException("ID of the show is not specified");
        }
        this.showId = showId;
        this.lastWatchedDate = lastWatchedDate != null ? lastWatchedDate : LocalDate.now();
    }

    public String getShowId() {
        return showId;
    }

    public LocalDate getLastWatchedDate() {
        return lastWatchedDate;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ShowView showView = (ShowView) o;
        return Objects.equals(showId, showView.showId) &&
                Objects.equals(lastWatchedDate, showView.lastWatchedDate);
    }

    @Override
    public int hashCode() {
        return Objects.hash(showId, lastWatchedDate);
    }

    @Override
    public String toString() {
        return "ShowView{" +
                "showId=" + showId +
                ", lastWatchedDate=" + lastWatchedDate +
                '}';
    }
}
