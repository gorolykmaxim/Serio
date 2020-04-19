package org.serio.core.showstorage;

import java.util.*;

/**
 * A TV-show.
 */
public class Show {
    private final ShowMetaData metaData;
    private final List<Episode> episodes;

    /**
     * @see Show#createNew(String, String, List)
     */
    public static Show createNew(String name, List<Episode> episodes) {
        return createNew(name, null, episodes);
    }

    /**
     * Create a new show.
     *
     * @param name name of the show
     * @param thumbnailUri URL to the thumbnail image of the show
     * @param episodes list of episodes of the show
     * @return created show
     */
    public static Show createNew(String name, String thumbnailUri, List<Episode> episodes) {
        return new Show(new ShowMetaData(null, name, thumbnailUri), episodes);
    }

    /**
     * Construct a show off of it's meta-data and a list of episodes.
     *
     * @param metaData meta-data of the show
     * @param episodes episodes of the show
     */
    public Show(ShowMetaData metaData, List<Episode> episodes) {
        this.metaData = metaData;
        this.episodes = Collections.unmodifiableList(new ArrayList<>(episodes));
    }

    /**
     * @see ShowMetaData#getId()
     */
    public UUID getId() {
        return metaData.getId();
    }

    /**
     * @see ShowMetaData#getName()
     */
    public String getName() {
        return metaData.getName();
    }

    /**
     * @see ShowMetaData#getThumbnailUrl()
     */
    public String getThumbnailUrl() {
        return metaData.getThumbnailUrl();
    }

    /**
     * Get the whole meta-data of the show.
     *
     * @return meta-data of this show
     */
    public ShowMetaData getMetaData() {
        return metaData;
    }

    /**
     * Get list of episodes of the show.
     *
     * @return episodes of this show
     */
    public List<Episode> getEpisodes() {
        return episodes;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Show show = (Show) o;
        return Objects.equals(metaData, show.metaData) &&
                Objects.equals(episodes, show.episodes);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(metaData, episodes);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "Show{" +
                "metaData=" + metaData +
                ", episodes=" + episodes +
                '}';
    }
}
