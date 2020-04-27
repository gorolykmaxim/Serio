package org.serio.core.applicationcontroller.model;

import org.serio.core.shows.WatchableShow;

import java.util.List;
import java.util.Objects;
import java.util.UUID;
import java.util.stream.Collectors;

public class DisplayableShow {
    private final DisplayableShowMetaData metaData;
    private final List<DisplayableEpisode> episodes;

    public DisplayableShow(WatchableShow show, DateFormat dateFormat) {
        metaData = new DisplayableShowMetaData(show.getMetaData(), dateFormat);
        episodes = show.getEpisodes()
                .stream()
                .map(DisplayableEpisode::new)
                .collect(Collectors.toList());
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
        return episodes;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DisplayableShow that = (DisplayableShow) o;
        return Objects.equals(metaData, that.metaData) &&
                Objects.equals(episodes, that.episodes);
    }

    @Override
    public int hashCode() {
        return Objects.hash(metaData, episodes);
    }

    @Override
    public String toString() {
        return "DisplayableShow{" +
                "metaData=" + metaData +
                ", episodes=" + episodes +
                '}';
    }
}
