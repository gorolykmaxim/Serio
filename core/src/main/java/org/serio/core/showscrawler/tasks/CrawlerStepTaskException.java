package org.serio.core.showscrawler.tasks;

import org.serio.core.showscrawler.crawler.step.CrawlerStep;

public class CrawlerStepTaskException extends RuntimeException {
    public CrawlerStepTaskException(CrawlerStep step, Throwable cause) {
        super(String.format("Failed to execute %s. Reason: %s", step, cause.getMessage()), cause);
    }
}
