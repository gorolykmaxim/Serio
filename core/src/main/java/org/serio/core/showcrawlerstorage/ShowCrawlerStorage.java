package org.serio.core.showcrawlerstorage;

import java.util.UUID;
import java.util.concurrent.Future;

/**
 * Storage used to store show crawler configurations.
 */
public interface ShowCrawlerStorage {
    /**
     * Find the crawler, used to crawl the show with the specified ID.
     *
     * @param showId ID of the show
     * @return show crawler used to crawl the show. Can't be null. If the crawler for an existing show does not
     * exist - an exception will be thrown.
     */
    Future<String> findShowCrawlerByShowId(UUID showId);

    /**
     * Save the specified crawler in the storage, while binding it to the show with the specified ID.
     *
     * @param showId ID of the show that was crawled using the crawler
     * @param showCrawler show crawler to save
     */
    void saveShowCrawler(UUID showId, String showCrawler);

    /**
     * Remove show crawler, used to crawl the show with the specified ID, from the storage.
     *
     * @param showId ID of the show
     */
    void deleteShowCrawlerByShowId(UUID showId);
}
