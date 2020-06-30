package org.serio.core.showscrawler.crawler;

import org.apache.commons.lang3.StringUtils;

public class CrawlerStepException extends IllegalArgumentException {
    public CrawlerStepException(int stepNumber, String crawlerType, Throwable cause) {
        super(String.format("Step #%d of %s crawler is invalid: %s", stepNumber, StringUtils.defaultString(crawlerType, "specified"), cause.getMessage()), cause);
    }

    protected CrawlerStepException(String message) {
        super(message);
    }
}
