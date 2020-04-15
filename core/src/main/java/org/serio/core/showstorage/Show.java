package org.serio.core.showstorage;

import org.apache.commons.lang3.StringUtils;

import java.util.*;

public class Show {
    private final UUID id;
    private final String name;
    private final String thumbnailUrl;
    private final List<Episode> episodes;

    public static Show createNew(String name, List<Episode> episodes) {
        return createNew(name, null, episodes);
    }

    public static Show createNew(String name, String thumbnailUri, List<Episode> episodes) {
        return new Show(UUID.randomUUID(), name, thumbnailUri, episodes);
    }

    public Show(UUID id, String name, String thumbnailUrl, List<Episode> episodes) {
        if (id == null) {
            throw new IllegalArgumentException("ID of the show is not specified");
        }
        if (StringUtils.isEmpty(name)) {
            throw new IllegalArgumentException("Name of the show is not specified or empty");
        }
        this.id = id;
        this.name = name;
        this.thumbnailUrl = thumbnailUrl != null ? thumbnailUrl : "";
        this.episodes = new ArrayList<>(episodes);
    }

    public UUID getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getThumbnailUrl() {
        return thumbnailUrl;
    }

    public List<Episode> getEpisodes() {
        return Collections.unmodifiableList(episodes);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Show show = (Show) o;
        return Objects.equals(id, show.id) &&
                Objects.equals(name, show.name) &&
                Objects.equals(thumbnailUrl, show.thumbnailUrl) &&
                Objects.equals(episodes, show.episodes);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, name, thumbnailUrl, episodes);
    }

    @Override
    public String toString() {
        return "Show{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", thumbnailUri='" + thumbnailUrl + '\'' +
                ", episodes=" + episodes +
                '}';
    }
}
