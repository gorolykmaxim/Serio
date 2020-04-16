package org.serio.core.watchhistory;

import java.time.Duration;
import java.time.LocalDate;
import java.util.Objects;

public class EpisodeView {
    private final String episodeId;
    private final Duration watchProgress;
    private final LocalDate lastWatchDate;

    public EpisodeView(String episodeId) {
        this(episodeId, null, null);
    }

    public EpisodeView(String episodeId, Duration watchProgress) {
        this(episodeId, watchProgress, null);
    }

    public EpisodeView(String episodeId, Duration watchProgress, LocalDate lastWatchDate) {
        if (episodeId == null) {
            throw new IllegalArgumentException("Episode ID is not specified");
        }
        this.episodeId = episodeId;
        this.watchProgress = watchProgress != null ? watchProgress : Duration.ZERO;
        this.lastWatchDate = lastWatchDate != null ? lastWatchDate : LocalDate.now();
    }

    public String getEpisodeId() {
        return episodeId;
    }

    public Duration getWatchProgress() {
        return watchProgress;
    }

    public LocalDate getLastWatchDate() {
        return lastWatchDate;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        EpisodeView that = (EpisodeView) o;
        return Objects.equals(episodeId, that.episodeId) &&
                Objects.equals(watchProgress, that.watchProgress) &&
                Objects.equals(lastWatchDate, that.lastWatchDate);
    }

    @Override
    public int hashCode() {
        return Objects.hash(episodeId, watchProgress, lastWatchDate);
    }

    @Override
    public String toString() {
        return "EpisodeView{" +
                "episodeId='" + episodeId + '\'' +
                ", watchProgress=" + watchProgress +
                ", lastWatchDate=" + lastWatchDate +
                '}';
    }
}
