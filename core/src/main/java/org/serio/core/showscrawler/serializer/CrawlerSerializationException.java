package org.serio.core.showscrawler.serializer;

public class CrawlerSerializationException extends RuntimeException {
    public CrawlerSerializationException(Object crawler, Throwable cause) {
        super(String.format("Failed to serialize %s. Reason: %s", crawler, cause.getMessage()), cause);
    }
}
