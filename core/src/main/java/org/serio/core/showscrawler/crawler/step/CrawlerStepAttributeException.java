package org.serio.core.showscrawler.crawler.step;

public class CrawlerStepAttributeException extends IllegalArgumentException {
    public CrawlerStepAttributeException(String crawlerType, String attributeName) {
        super(String.format("'%s' crawler step must have '%s' attribute specified", crawlerType, attributeName));
    }
}
