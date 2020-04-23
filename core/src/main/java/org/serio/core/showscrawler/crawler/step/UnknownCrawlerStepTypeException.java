package org.serio.core.showscrawler.crawler.step;

import java.util.Arrays;

public class UnknownCrawlerStepTypeException extends CrawlerStepTypeException {
    public UnknownCrawlerStepTypeException(String type, String ...knownTypes) {
        super(String.format("Unknown crawler step type '%s'. Supported types are: %s", type,  Arrays.asList(knownTypes)));
    }
}
