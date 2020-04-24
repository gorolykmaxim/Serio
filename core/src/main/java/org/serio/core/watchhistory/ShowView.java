package org.serio.core.watchhistory;

import java.time.LocalDateTime;
import java.util.Objects;

/**
 * User's view of a show.
 *
 * <p>Indicates that the user has watched the corresponding show.</p>
 */
public class ShowView {
    private final String showId;
    private final LocalDateTime lastWatchedDate;

    /**
     * @see ShowView#ShowView(String, LocalDateTime)
     */
    public ShowView(String showId) {
        this(showId, null);
    }

    /**
     * Construct a view of a show.
     *
     * @param showId ID of the show, that has been watched
     * @param lastWatchedDate the date when the show has been watched the last time. If not specified - today's date
     *                        will be used.
     */
    public ShowView(String showId, LocalDateTime lastWatchedDate) {
        if (showId == null) {
            throw new IllegalArgumentException("ID of the show is not specified");
        }
        this.showId = showId;
        this.lastWatchedDate = lastWatchedDate != null ? lastWatchedDate : LocalDateTime.now();
    }

    /**
     * Get ID of the show, that has been watched.
     *
     * @return ID of the show
     */
    public String getShowId() {
        return showId;
    }

    /**
     * Get the date when this view was created.
     *
     * @return date of the last time the corresponding show has been watched
     */
    public LocalDateTime getLastWatchedDate() {
        return lastWatchedDate;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ShowView showView = (ShowView) o;
        return Objects.equals(showId, showView.showId) &&
                Objects.equals(lastWatchedDate, showView.lastWatchedDate);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(showId, lastWatchedDate);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "ShowView{" +
                "showId=" + showId +
                ", lastWatchedDate=" + lastWatchedDate +
                '}';
    }
}
