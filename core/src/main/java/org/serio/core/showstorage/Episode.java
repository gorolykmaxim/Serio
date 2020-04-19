package org.serio.core.showstorage;

import java.util.Objects;

/**
 * An episode of a show.
 */
public class Episode {
    private final long id;
    private final String name;
    private final String videoUrl;

    /**
     * @see Episode#Episode(long, String, String)
     */
    public Episode(long id, String videoUrl) {
        this(id, null, videoUrl);
    }

    /**
     * Construct an episode of some show.
     *
     * @param id ID of the episode, which is also it's position in the list of episode's of the corresponding show.
     *           The value should be a positive integer that is greater than 0.
     * @param name name of the episode. If not specified - the name will be automatically generated based off
     *             of the episode's ID
     * @param videoUrl URL to the video file, containing the episode. Can't be null.
     */
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

    /**
     * Get ID of the episode.
     *
     * <p>ID of an episode is the episode's serial number in its show's episode list.</p>
     *
     * @return ID of the episode
     */
    public long getId() {
        return id;
    }

    /**
     * Get name of the episode.
     *
     * @return name of the episode
     */
    public String getName() {
        return name;
    }

    /**
     * Get URL to the video file, containing the episode
     *
     * @return video URL to this episode
     */
    public String getVideoUrl() {
        return videoUrl;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Episode episode = (Episode) o;
        return id == episode.id &&
                Objects.equals(name, episode.name) &&
                Objects.equals(videoUrl, episode.videoUrl);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(id, name, videoUrl);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "Episode{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", videoUrl='" + videoUrl + '\'' +
                '}';
    }
}
