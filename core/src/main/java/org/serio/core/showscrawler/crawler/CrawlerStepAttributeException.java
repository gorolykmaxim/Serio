package org.serio.core.showscrawler.crawler;

import org.apache.commons.lang3.StringUtils;

import java.util.Collection;

public class CrawlerStepAttributeException extends IllegalArgumentException {
    public CrawlerStepAttributeException(String crawlerType, Collection<String> attributeNames) {
        super(String.format("'%s' crawler step must have following attributes specified: %s", crawlerType, StringUtils.join(", ", attributeNames)));
    }
}
