package org.serio.core.showscrawler.crawler.step;

public class CrawlerStepTypeNotSpecifiedException extends CrawlerStepTypeException {
    public CrawlerStepTypeNotSpecifiedException() {
        super("'type' of the crawler step not specified");
    }
}
