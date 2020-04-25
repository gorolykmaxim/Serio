package org.serio.core.showscrawler;

import org.apache.commons.lang3.StringUtils;
import org.serio.core.httpclient.HttpClient;
import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.core.showcrawlerlogstorage.ShowCrawlerLogStorage;
import org.serio.core.showcrawlerstorage.ShowCrawlerStorage;
import org.serio.core.showscrawler.crawler.Crawler;
import org.serio.core.showscrawler.crawler.ShowCrawler;
import org.serio.core.showscrawler.serializer.CrawlerSerializer;
import org.serio.core.showscrawler.tasks.CrawlerTaskException;
import org.serio.core.showscrawler.tasks.CrawlerTask;
import org.serio.core.showstorage.Episode;
import org.serio.core.showstorage.Show;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

/**
 * Crawls tv shows from various web sources.
 */
public class ShowsCrawler {
    private final ShowCrawlerStorage showCrawlerStorage;
    private final ShowCrawlerLogStorage showCrawlerLogStorage;
    private final HttpClient httpClient;
    private final CrawlerSerializer crawlerSerializer;
    private final ExecutorService executorService;
    private final long crawlerLogDetailsLength;

    /**
     * Construct a shows crawler module.
     *
     * @param showCrawlerStorage storage, that will be used to store show crawlers
     * @param showCrawlerLogStorage storage, that will be used to store show crawling logs
     * @param httpClient http client, that will be used by crawlers to access web resources
     * @param crawlerLogDetailsLength maximum length of each crawling log entry input and output information fields
     */
    public ShowsCrawler(ShowCrawlerStorage showCrawlerStorage, ShowCrawlerLogStorage showCrawlerLogStorage,
                        HttpClient httpClient, long crawlerLogDetailsLength) {
        this.showCrawlerStorage = showCrawlerStorage;
        this.showCrawlerLogStorage = showCrawlerLogStorage;
        this.httpClient = httpClient;
        this.crawlerLogDetailsLength = crawlerLogDetailsLength;
        crawlerSerializer = new CrawlerSerializer();
        executorService = Executors.newCachedThreadPool();
    }

    /**
     * Get partially deserialized representation of a crawler of the specified show.
     *
     * @param showId ID of the show
     * @return partially deserialized show crawler
     */
    public SerializedShowCrawlerParts getSerializedPartsOfCrawlerOfShow(String showId) {
        try {
            String serializedShowCrawler = showCrawlerStorage.findShowCrawlerByShowId(showId).get();
            ShowCrawler showCrawler = crawlerSerializer.deserializeShowCrawler(serializedShowCrawler);
            return new SerializedShowCrawlerParts(
                    showCrawler.getShowName(),
                    crawlerSerializer.serializeCrawler(showCrawler.getThumbnailCrawler()),
                    crawlerSerializer.serializeCrawler(showCrawler.getEpisodeVideoCrawler()),
                    crawlerSerializer.serializeCrawler(showCrawler.getEpisodeNameCrawler())
            );
        } catch (Exception e) {
            throw new ShowCrawlerLookupException(showId, e);
        }
    }

    /**
     * Get serialized representation of a crawler of the specified show.
     *
     * @param showId ID of the show
     * @return serialized show crawler
     */
    public String getSerializedCrawlerOfShow(String showId) {
        try {
            return showCrawlerStorage.findShowCrawlerByShowId(showId).get();
        } catch (Exception e) {
            throw new ShowCrawlerLookupException(showId, e);
        }
    }

    /**
     * Crawl a tv show according to the specified show crawler configuration.
     *
     * <p>Both crawler and the log, generate during the crawl, will be stored in the corresponding storages
     * after the completion of the crawling process.</p>
     *
     * @param showName name of the show to crawl
     * @param serializedThumbnailCrawler serialized configuration of the show's thumbnail crawler
     * @param serializedEpisodeVideoCrawler serialized configuration of the show's episode video crawler
     * @param serializedEpisodeNameCrawler serialized configuration of the show's episode name crawler
     * @return crawled tv show
     */
    public Show crawlShowAndSaveCrawler(String showName, String serializedThumbnailCrawler,
                                        String serializedEpisodeVideoCrawler, String serializedEpisodeNameCrawler) {
        try {
            Crawler episodeVideoCrawler = StringUtils.isNotEmpty(serializedEpisodeVideoCrawler) ? crawlerSerializer.deserializeCrawler(serializedEpisodeVideoCrawler) : null;
            Crawler thumbnailCrawler = StringUtils.isNotEmpty(serializedThumbnailCrawler) ? crawlerSerializer.deserializeCrawler(serializedThumbnailCrawler) : null;
            Crawler episodeNameCrawler = StringUtils.isNotEmpty(serializedEpisodeNameCrawler) ? crawlerSerializer.deserializeCrawler(serializedEpisodeNameCrawler) : null;
            return crawlShow(new ShowCrawler(showName, thumbnailCrawler, episodeVideoCrawler, episodeNameCrawler), true);
        } catch (Exception e) {
            throw ShowCrawlerException.failedToCrawlShow(showName, e);
        }
    }

    /**
     * Crawl a tv show according to the specified show crawler configuration.
     *
     * <p>Both crawler and the log, generate during the crawl, will be stored in the corresponding storages
     * after the completion of the crawling process.</p>
     *
     * @param serializedShowCrawler serialized configuration of a whole show crawler
     * @return crawled tv show
     */
    public Show crawlShowAndSaveCrawler(String serializedShowCrawler) {
        try {
            return crawlShow(crawlerSerializer.deserializeShowCrawler(serializedShowCrawler), true);
        } catch (Exception e) {
            throw ShowCrawlerException.failedToExecuteCrawler(serializedShowCrawler, e);
        }
    }

    /**
     * Re-crawl the specified tv-show.
     *
     * <p>Crawler for the specified show must already existing in the show crawler storage.</p>
     *
     * @param showId ID of the show
     * @return crawled tv show
     */
    public Show crawlShow(String showId) {
        try {
            String serializedShowCrawler = showCrawlerStorage.findShowCrawlerByShowId(showId).get();
            return crawlShow(crawlerSerializer.deserializeShowCrawler(serializedShowCrawler), false);
        } catch (Exception e) {
            throw ShowCrawlerException.failedToCrawlShowWithId(showId, e);
        }
    }

    private Show crawlShow(ShowCrawler showCrawler, boolean saveCrawler) throws ExecutionException, InterruptedException {
        List<CrawlLogEntry> log = new ArrayList<>();
        Future<CrawlingResult> thumbnailResultsFuture = executorService.submit(new CrawlerTask(showCrawler.getThumbnailCrawler(), httpClient, crawlerLogDetailsLength));
        Future<CrawlingResult> episodeVideosFuture = executorService.submit(new CrawlerTask(showCrawler.getEpisodeVideoCrawler(), httpClient, crawlerLogDetailsLength));
        CrawlingResult thumbnailResults = thumbnailResultsFuture.get();
        String thumbnailUrl = thumbnailResults.getFirstOutputLine();
        log.add(new CrawlLogEntry("Crawling thumbnail"));
        log.addAll(thumbnailResults.getLog());
        CrawlingResult episodeVideoUrlResults = episodeVideosFuture.get();
        List<String> episodeVideoUrls = episodeVideoUrlResults.getOutput();
        log.add(new CrawlLogEntry("Crawling episode videos"));
        log.addAll(episodeVideoUrlResults.getLog());
        List<Episode> episodes = new ArrayList<>();
        if (showCrawler.getEpisodeNameCrawler().getSteps().isEmpty()) {
            for (int i = 0; i < episodeVideoUrls.size(); i++) {
                episodes.add(new Episode(i + 1, episodeVideoUrls.get(i)));
            }
        } else {
            Future<CrawlingResult> episodeNamesFuture = executorService.submit(new CrawlerTask(showCrawler.getEpisodeNameCrawler(), httpClient, crawlerLogDetailsLength, episodeVideoUrls));
            CrawlingResult episodeNameResults = episodeNamesFuture.get();
            log.add(new CrawlLogEntry("Crawling episode names"));
            log.addAll(episodeNameResults.getLog());
            List<String> episodeNames = episodeNameResults.getOutput();
            if (episodeVideoUrls.size() != episodeNames.size()) {
                throw new EpisodeVideosToNamesMismatchException(episodeVideoUrls, episodeNames);
            }
            for (int i = 0; i < episodeVideoUrls.size(); i++) {
                episodes.add(new Episode(i + 1, episodeNames.get(i), episodeVideoUrls.get(i)));
            }
        }
        Show show = Show.createNew(showCrawler.getShowName(), thumbnailUrl, episodes);
        if (saveCrawler) {
            String serializedShowCrawler = crawlerSerializer.serializeShowCrawler(showCrawler);
            showCrawlerStorage.saveShowCrawler(show.getId().toString(), serializedShowCrawler);
        }
        showCrawlerLogStorage.saveCrawlingLogOfTheShow(show.getId().toString(), log);
        return show;
    }

    /**
     * Execute the specified crawler and return its results.
     *
     * <p>This is a simple crawler preview operation, which will not affect any of existing state.</p>
     *
     * @param serializedCrawler serialized crawler configuration
     * @return crawler execution results
     */
    public CrawlingResult previewCrawler(String serializedCrawler) {
        try {
            Crawler crawler = crawlerSerializer.deserializeCrawler(serializedCrawler);
            CrawlerTask crawlerTask = new CrawlerTask(crawler, httpClient, crawlerLogDetailsLength);
            return crawlerTask.call();
        } catch (CrawlerTaskException e) {
            throw e;
        } catch (Exception e) {
            throw new CrawlerTaskException(serializedCrawler, e);
        }
    }

    /**
     * Get the log of the last crawling attempt of the specified show.
     *
     * @param showId ID of the show
     * @return last crawling log of the specified show. Will be empty if the specified show has not been crawled yet.
     */
    public Optional<List<CrawlLogEntry>> getLastCrawlingLogOfShow(String showId) {
        try {
            return showCrawlerLogStorage.getLastCrawlingLogOfTheShow(showId).get();
        } catch (Exception e) {
            throw new CrawlingLogLookupException(showId, e);
        }
    }

    /**
     * Delete the crawler of the specified show.
     *
     * @param showId ID of the show
     */
    public void deleteCrawlerOfShow(String showId) {
        showCrawlerStorage.deleteShowCrawlerByShowId(showId);
    }
}