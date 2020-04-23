package org.serio.core.showscrawler;

public class CrawlingLogLookupException extends RuntimeException {
    public CrawlingLogLookupException(String showId, Throwable cause) {
        super(String.format("Failed to find crawl log of the show with ID '%s'. Reason: %s", showId, cause.getMessage()), cause);
    }
}
