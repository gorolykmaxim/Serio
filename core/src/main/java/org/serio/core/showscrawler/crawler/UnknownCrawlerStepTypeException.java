package org.serio.core.showscrawler.crawler;

import org.apache.commons.lang3.StringUtils;

import java.util.Collection;

public class UnknownCrawlerStepTypeException extends CrawlerStepTypeException {
    public UnknownCrawlerStepTypeException(String type, Collection<String> existingTypes) {
        super(String.format("Unknown crawler step type '%s'. Supported types are: %s", type, StringUtils.join(existingTypes, ", ")));
    }
}
