package org.serio.core.shows;

import org.serio.core.showstorage.Episode;
import org.serio.core.watchhistory.EpisodeView;

import java.time.Duration;
import java.time.LocalDate;
import java.util.Objects;
import java.util.Optional;

public class WatchableEpisode {
    private final Episode episode;
    private final EpisodeView view;

    public WatchableEpisode(Episode episode, EpisodeView view) {
        if (episode == null) {
            throw new IllegalArgumentException("Episode is not specified");
        }
        this.episode = episode;
        this.view = view;
    }

    public long getId() {
        return episode.getId();
    }

    public String getName() {
        return episode.getName();
    }

    public String getVideoUrl() {
        return episode.getVideoUrl();
    }

    public Duration getWatchProgress() {
        return view != null ? view.getWatchProgress() : Duration.ZERO;
    }

    public Optional<LocalDate> getLastWatchDate() {
        return Optional.ofNullable(view != null ? view.getLastWatchDate() : null);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        WatchableEpisode that = (WatchableEpisode) o;
        return Objects.equals(episode, that.episode) &&
                Objects.equals(view, that.view);
    }

    @Override
    public int hashCode() {
        return Objects.hash(episode, view);
    }

    @Override
    public String toString() {
        return "WatchableEpisode{" +
                "episode=" + episode +
                ", view=" + view +
                '}';
    }
}
