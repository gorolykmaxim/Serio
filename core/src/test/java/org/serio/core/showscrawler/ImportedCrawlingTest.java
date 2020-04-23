package org.serio.core.showscrawler;

import org.junit.Before;

public class ImportedCrawlingTest extends BaseCrawlingTest {
    @Override
    @Before
    public void setUp() throws Exception {
        super.setUp();
        method = (showName, thumbnailCrawler, episodeVideoCrawler, episodeNameCrawler) -> showsCrawler.crawlShowAndSaveCrawler(createSerializedShowCrawler(showName, thumbnailCrawler, episodeVideoCrawler, episodeNameCrawler));
    }

    protected String createSerializedShowCrawler(String showName, String thumbnailCrawler, String episodeVideoCrawler, String episodeNameCrawler) {
        if (thumbnailCrawler == null && episodeNameCrawler == null) {
            return String.format(BARE_MINIMUM_SHOW_CRAWLER_TEMPLATE, showName, episodeVideoCrawler);
        } else if (showName == null) {
            return String.format(SHOW_CRAWLER_WITHOUT_SHOW_NAME, thumbnailCrawler, episodeVideoCrawler, episodeNameCrawler);
        } else if (episodeVideoCrawler == null) {
            return String.format(SHOW_CRAWLER_WITHOUT_EPISODE_VIDEO_CRAWLER, thumbnailCrawler, episodeNameCrawler);
        } else {
            return String.format(COMPLETE_SHOW_CRAWLER_TEMPLATE, showName, thumbnailCrawler, episodeVideoCrawler, episodeNameCrawler);
        }
    }
}
