package org.serio.core.applicationcontroller.model;

import org.serio.core.shows.WatchableShow;

import java.util.List;
import java.util.Objects;
import java.util.Optional;
import java.util.UUID;
import java.util.stream.Collectors;

public class DisplayableShow {
    private final WatchableShow show;
    private final DisplayableShowMetaData metaData;

    public DisplayableShow(WatchableShow show, DateFormat dateFormat) {
        this.show = show;
        metaData = new DisplayableShowMetaData(show.getMetaData(), dateFormat);
    }

    public UUID getId() {
        return metaData.getId();
    }

    public String getName() {
        return metaData.getName();
    }

    public String getThumbnail() {
        return metaData.getThumbnail();
    }

    public String getLastWatched() {
        return metaData.getLastWatched();
    }

    public List<DisplayableEpisode> getEpisodes() {
        return show.getEpisodes()
                .stream()
                .map(DisplayableEpisode::new)
                .collect(Collectors.toList());
    }

    public Optional<DisplayableEpisode> getEpisodeById(int episodeId) {
        return show.getEpisodeById(episodeId).map(DisplayableEpisode::new);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DisplayableShow show1 = (DisplayableShow) o;
        return Objects.equals(show, show1.show) &&
                Objects.equals(metaData, show1.metaData);
    }

    @Override
    public int hashCode() {
        return Objects.hash(show, metaData);
    }

    @Override
    public String toString() {
        return "DisplayableShow{" +
                "show=" + show +
                ", metaData=" + metaData +
                '}';
    }
}
