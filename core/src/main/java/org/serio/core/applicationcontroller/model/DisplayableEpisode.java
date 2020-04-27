package org.serio.core.applicationcontroller.model;

import org.serio.core.shows.WatchableEpisode;

import java.util.Objects;

public class DisplayableEpisode {
    private final WatchableEpisode episode;

    public DisplayableEpisode(WatchableEpisode episode) {
        if (episode == null) {
            throw new IllegalArgumentException("Episode is not specified");
        }
        this.episode = episode;
    }

    public int getId() {
        return episode.getId();
    }

    public boolean isWatched() {
        return episode.hasBeenWatched();
    }

    public String getName() {
        return episode.getName();
    }

    public String getVideoUrl() {
        return episode.getVideoUrl();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DisplayableEpisode that = (DisplayableEpisode) o;
        return Objects.equals(episode, that.episode);
    }

    @Override
    public int hashCode() {
        return Objects.hash(episode);
    }

    @Override
    public String toString() {
        return "DisplayableEpisode{" +
                "episode=" + episode +
                '}';
    }
}
