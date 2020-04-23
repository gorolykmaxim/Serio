package org.serio.core.showscrawler.serializer;

public class CrawlerDeserializationException extends RuntimeException {
    public CrawlerDeserializationException(String crawler, Throwable cause) {
        super(String.format("Failed to deserialize %s. Reason: %s", crawler, cause.getMessage()), cause);
    }
}
