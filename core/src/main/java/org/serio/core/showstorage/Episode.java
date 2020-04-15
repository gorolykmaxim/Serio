package org.serio.core.showstorage;

import java.util.Objects;

public class Episode {
    private final long id;
    private final String name;
    private final String videoUrl;

    public Episode(long id, String videoUrl) {
        this(id, null, videoUrl);
    }

    public Episode(long id, String name, String videoUrl) {
        if (id < 1) {
            throw new IllegalArgumentException("ID of an episode is it's positional number and should start from 1. Specified - " + id);
        }
        if (videoUrl == null) {
            throw new IllegalArgumentException("Video URL of the episode not specified");
        }
        this.id = id;
        this.name = name != null ? name : "Episode " + id;
        this.videoUrl = videoUrl;
    }

    public long getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getVideoUrl() {
        return videoUrl;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Episode episode = (Episode) o;
        return id == episode.id &&
                Objects.equals(name, episode.name) &&
                Objects.equals(videoUrl, episode.videoUrl);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, name, videoUrl);
    }

    @Override
    public String toString() {
        return "Episode{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", videoUrl='" + videoUrl + '\'' +
                '}';
    }
}
