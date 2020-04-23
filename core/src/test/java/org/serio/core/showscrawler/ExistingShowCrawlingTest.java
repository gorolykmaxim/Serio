package org.serio.core.showscrawler;

import org.junit.Before;

import java.util.concurrent.CompletableFuture;

import static org.mockito.Mockito.*;

public class ExistingShowCrawlingTest extends ImportedCrawlingTest {
    @Override
    @Before
    public void setUp() throws Exception {
        super.setUp();
        method = (showName, thumbnailCrawler, episodeVideoCrawler, episodeNameCrawler) -> {
            String serializedCrawler = createSerializedShowCrawler(showName, thumbnailCrawler, episodeVideoCrawler, episodeNameCrawler);
            when(showCrawlerStorage.findShowCrawlerByShowId(showId))
                    .thenReturn(CompletableFuture.completedFuture(serializedCrawler));
            return showsCrawler.crawlShow(showId);
        };
    }

    @Override
    public void shouldCrawlAndSaveInStorageCompleteCrawler() {
        // when
        method.call(SHOW_NAME, COMPLETE_THUMBNAIL_CRAWLER, COMPLETE_VIDEO_CRAWLER, COMPLETE_NAME_CRAWLER);
        // then
        // While crawling an existing show using the corresponding existing crawler - the latter one should not be
        // re-saved in storage.
        verify(showCrawlerStorage, never()).saveShowCrawler(any(), any());
    }

    @Override
    public void shouldCrawlAndSaveInStorageBareMinimumCrawler() {
        // when
        method.call(SHOW_NAME, null, COMPLETE_VIDEO_CRAWLER, null);
        // then
        // While crawling an existing show using the corresponding existing crawler - the latter one should not be
        // re-saved in storage.
        verify(showCrawlerStorage, never()).saveShowCrawler(any(), any());
    }

    @Override
    public void shouldCrawlAndSaveInStorageAnyCrawlerEvenIfLogSavingFails() {
        // Show crawler is not getting re-saved in storage after crawling an existing show.
    }
}
