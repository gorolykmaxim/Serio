package org.serio.core.showscrawler.crawler;

import org.apache.commons.lang3.StringUtils;

import java.util.Objects;

/**
 * Crawler of a show.
 *
 * <p>Show crawler consists of 3 parts:</p>
 * <ul>
 *     <li>crawler of video file URLs of the actual episodes of the show</li>
 *     <li>(optional) crawler of the URL to the thumbnail image, that will represent the show</li>
 *     <li>(optional) crawler of names of episodes of the show</li>
 * </ul>
 *
 * <p>Thumbnail crawler starts with no input data passed to it. From the array of strings, produced by it,
 * the first string is used as a URL to the thumbnail image.</p>
 *
 * <p>Episode video crawler starts with no input data passed to it. Each string from the array, produced by it,
 * is treated as a URL to a video file of a corresponding episode. Video URLs are assigned to the episodes
 * in the order in which they were produced by the crawler, meaning the first URL will be considered the URL
 * to the first episode, the second one - the second one etc.</p>
 *
 * <p>All crawlers of the show behave the same except for the episode name crawler. Episode name crawler
 * receives list of episode video URLs, crawled by the corresponding crawler. This is done to reduce the amount
 * of unnecessary network calls to the actual website, that hosts the show, since in a lot of cases,
 * episode video URL contains the name of the actual episode. Video crawler results are passed to this crawler
 * in the same order they were produced.</p>
 */
public class ShowCrawler {
    private final String showName;
    private final Crawler thumbnailCrawler;
    private final Crawler episodeVideoCrawler;
    private final Crawler episodeNameCrawler;

    /**
     * Construct a show crawler.
     *
     * @param showName name of the show, that will be crawled by this crawler. Can't be null or empty.
     * @param thumbnailCrawler crawler that will be used to crawl the URL of the show's thumbnail. Can be null.
     * @param episodeVideoCrawler crawler that will be used to crawl URLs of episode video files. Can't be null.
     * @param episodeNameCrawler crawler that will be used to crawl names of episodes. Can be null.
     */
    public ShowCrawler(String showName, Crawler thumbnailCrawler, Crawler episodeVideoCrawler, Crawler episodeNameCrawler) {
        if (StringUtils.isEmpty(showName)) {
            throw new IllegalArgumentException("Show name is empty or not specified");
        }
        if (episodeVideoCrawler == null) {
            throw new IllegalArgumentException("Episode video crawler is not specified");
        }
        this.showName = showName;
        this.thumbnailCrawler = thumbnailCrawler != null ? thumbnailCrawler : new Crawler();
        this.episodeVideoCrawler = episodeVideoCrawler;
        this.episodeNameCrawler = episodeNameCrawler != null ? episodeNameCrawler : new Crawler();
    }

    /**
     * Get name of the show, crawled by this crawler.
     *
     * @return name of the show
     */
    public String getShowName() {
        return showName;
    }

    /**
     * Get crawler of the show's thumbnail URL.
     *
     * @return crawler of the thumbnail URL.
     */
    public Crawler getThumbnailCrawler() {
        return thumbnailCrawler;
    }

    /**
     * Get crawler of episode video URLs.
     *
     * @return crawler of episode video URLs
     */
    public Crawler getEpisodeVideoCrawler() {
        return episodeVideoCrawler;
    }

    /**
     * Get crawler of episode names.
     *
     * @return crawler of episode names
     */
    public Crawler getEpisodeNameCrawler() {
        return episodeNameCrawler;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ShowCrawler crawler = (ShowCrawler) o;
        return Objects.equals(showName, crawler.showName) &&
                Objects.equals(thumbnailCrawler, crawler.thumbnailCrawler) &&
                Objects.equals(episodeVideoCrawler, crawler.episodeVideoCrawler) &&
                Objects.equals(episodeNameCrawler, crawler.episodeNameCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(showName, thumbnailCrawler, episodeVideoCrawler, episodeNameCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "ShowCrawler{" +
                "showName='" + showName + '\'' +
                ", thumbnailCrawler=" + thumbnailCrawler +
                ", episodeVideoCrawler=" + episodeVideoCrawler +
                ", episodeNameCrawler=" + episodeNameCrawler +
                '}';
    }
}
