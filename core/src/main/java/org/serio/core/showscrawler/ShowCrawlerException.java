package org.serio.core.showscrawler;

public class ShowCrawlerException extends RuntimeException {
    public static ShowCrawlerException failedToCrawlShow(String showName, Throwable cause) {
        return new ShowCrawlerException(String.format("crawl show '%s'", showName), cause);
    }

    public static ShowCrawlerException failedToCrawlShowWithId(String showId, Throwable cause) {
        return new ShowCrawlerException(String.format("crawl show with ID %s", showId), cause);
    }

    public static ShowCrawlerException failedToExecuteCrawler(String serializedCrawler, Throwable cause) {
        return new ShowCrawlerException(String.format("execute show crawler '%s'", serializedCrawler), cause);
    }

    private ShowCrawlerException(String message, Throwable cause) {
        super(String.format("Failed to %s. Reason: %s", message, cause.getMessage()), cause);
    }
}
