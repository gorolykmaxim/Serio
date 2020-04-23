package org.serio.core.showscrawler;

import org.junit.Before;

public class ManuallyConfiguredCrawlingTest extends BaseCrawlingTest {
    @Override
    @Before
    public void setUp() throws Exception {
        super.setUp();
        method = showsCrawler::crawlShowAndSaveCrawler;
    }
}
