package org.serio.core.showscrawler;

public class ShowCrawlerLookupException extends RuntimeException {
    public ShowCrawlerLookupException(String showId, Throwable cause) {
        super(String.format("Failed to load crawler of the show with ID %s. Reason: %s", showId, cause.getMessage()), cause);
    }
}
