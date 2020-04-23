package org.serio.core.showscrawler.tasks;

import org.serio.core.showscrawler.crawler.Crawler;

public class CrawlerTaskException extends RuntimeException {
    public CrawlerTaskException(Crawler crawler, Throwable cause) {
        this(crawler.toString(), cause);
    }
    public CrawlerTaskException(String crawler, Throwable cause) {
        super(String.format("Failed to execute %s. Reason: %s", crawler, cause.getMessage()), cause);
    }
}
