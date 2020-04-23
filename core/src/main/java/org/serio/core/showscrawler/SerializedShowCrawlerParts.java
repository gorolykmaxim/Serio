package org.serio.core.showscrawler;

import java.util.Objects;

/**
 * Partially deserialized representation of a show crawler, where each of it's crawlers is serialized.
 */
public class SerializedShowCrawlerParts {
    private final String showName;
    private final String serializedThumbnailCrawler;
    private final String serializedEpisodeVideoCrawler;
    private final String serializedEpisodeNameCrawler;

    /**
     * Construct a serialized show crawler parts.
     *
     * @param showName name of the show, crawled by the show crawler
     * @param serializedThumbnailCrawler serialized representation of the show thumbnail crawler
     * @param serializedEpisodeVideoCrawler serialized representation of the show episode video crawler
     * @param serializedEpisodeNameCrawler serialized representation of the show episode name crawler
     */
    public SerializedShowCrawlerParts(String showName, String serializedThumbnailCrawler, String serializedEpisodeVideoCrawler, String serializedEpisodeNameCrawler) {
        if (showName == null) {
            throw new IllegalArgumentException("Show name is not specified");
        }
        if (serializedThumbnailCrawler == null) {
            throw new IllegalArgumentException("Serialized thumbnail crawler is not specified");
        }
        if (serializedEpisodeVideoCrawler == null) {
            throw new IllegalArgumentException("Serialized episode video crawler is not specified");
        }
        if (serializedEpisodeNameCrawler == null) {
            throw new IllegalArgumentException("Serialized episode name crawler is not specified");
        }
        this.showName = showName;
        this.serializedThumbnailCrawler = serializedThumbnailCrawler;
        this.serializedEpisodeVideoCrawler = serializedEpisodeVideoCrawler;
        this.serializedEpisodeNameCrawler = serializedEpisodeNameCrawler;
    }

    /**
     * Get name of a show, crawled by the crawler.
     *
     * @return name of the crawled show
     */
    public String getShowName() {
        return showName;
    }

    /**
     * Get serialized representation of the show thumbnail crawler.
     *
     * @return serialized show thumbnail crawler
     */
    public String getSerializedThumbnailCrawler() {
        return serializedThumbnailCrawler;
    }

    /**
     * Get serialized representation of the show episode video crawler.
     *
     * @return serialized show episode video crawler
     */
    public String getSerializedEpisodeVideoCrawler() {
        return serializedEpisodeVideoCrawler;
    }

    /**
     * Get serialized representation of the show episode name crawler.
     *
     * @return serialized show episode name crawler
     */
    public String getSerializedEpisodeNameCrawler() {
        return serializedEpisodeNameCrawler;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        SerializedShowCrawlerParts that = (SerializedShowCrawlerParts) o;
        return Objects.equals(showName, that.showName) &&
                Objects.equals(serializedThumbnailCrawler, that.serializedThumbnailCrawler) &&
                Objects.equals(serializedEpisodeVideoCrawler, that.serializedEpisodeVideoCrawler) &&
                Objects.equals(serializedEpisodeNameCrawler, that.serializedEpisodeNameCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(showName, serializedThumbnailCrawler, serializedEpisodeVideoCrawler, serializedEpisodeNameCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "SerializedShowCrawlerParts{" +
                "showName='" + showName + '\'' +
                ", serializedThumbnailCrawler='" + serializedThumbnailCrawler + '\'' +
                ", serializedEpisodeVideoCrawler='" + serializedEpisodeVideoCrawler + '\'' +
                ", serializedEpisodeNameCrawler='" + serializedEpisodeNameCrawler + '\'' +
                '}';
    }
}
