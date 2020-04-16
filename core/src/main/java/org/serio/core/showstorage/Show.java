package org.serio.core.showstorage;

import java.util.*;

public class Show {
    private final ShowMetaData metaData;
    private final List<Episode> episodes;

    public static Show createNew(String name, List<Episode> episodes) {
        return createNew(name, null, episodes);
    }

    public static Show createNew(String name, String thumbnailUri, List<Episode> episodes) {
        return new Show(new ShowMetaData(null, name, thumbnailUri), episodes);
    }

    public Show(ShowMetaData metaData, List<Episode> episodes) {
        this.metaData = metaData;
        this.episodes = Collections.unmodifiableList(new ArrayList<>(episodes));
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

    public ShowMetaData getMetaData() {
        return metaData;
    }

    public List<Episode> getEpisodes() {
        return episodes;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Show show = (Show) o;
        return Objects.equals(metaData, show.metaData) &&
                Objects.equals(episodes, show.episodes);
    }

    @Override
    public int hashCode() {
        return Objects.hash(metaData, episodes);
    }

    @Override
    public String toString() {
        return "Show{" +
                "metaData=" + metaData +
                ", episodes=" + episodes +
                '}';
    }
}
