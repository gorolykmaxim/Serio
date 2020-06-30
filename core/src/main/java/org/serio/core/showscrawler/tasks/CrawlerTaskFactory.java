package org.serio.core.showscrawler.tasks;

import org.serio.core.showscrawler.crawler.Crawler;
import org.serio.core.showscrawler.crawler.CrawlerStepTypes;

import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Factory of {@link CrawlerTask}.
 */
public class CrawlerTaskFactory {
    private final CrawlerStepTypes crawlerStepTypes;
    private final long logDetailsLength;
    private final Map<String, CrawlerStepTask> stepTypeToTask;

    /**
     * Construct a crawler task factory.
     *
     * @param crawlerStepTypes will be supplied to each created {@link CrawlerTask}
     * @param logDetailsLength will be supplied to each created {@link CrawlerTask}
     */
    public CrawlerTaskFactory(CrawlerStepTypes crawlerStepTypes, long logDetailsLength) {
        this.crawlerStepTypes = crawlerStepTypes;
        stepTypeToTask = new HashMap<>();
        this.logDetailsLength = logDetailsLength;
    }

    /**
     * Specify a task each constructed {@link CrawlerTask} will execute to process a crawler step of the specified type.
     *
     * @param type type of a crawler step to process with the specified task
     * @param task task to process a crawler step with
     */
    public void addStepTask(String type, CrawlerStepTask task) {
        stepTypeToTask.put(type, task);
    }

    /**
     * @see CrawlerTaskFactory#create(Crawler, List)
     */
    public CrawlerTask create(Crawler crawler) {
        return create(crawler, null);
    }

    /**
     * Create a crawler task for the specified crawler.
     *
     * @param crawler crawler to execute by the constructed task
     * @param input   input to pass to the crawler task
     * @return task, that will execute the specified crawler
     */
    public CrawlerTask create(Crawler crawler, List<String> input) {
        return new CrawlerTask(crawler, input, logDetailsLength, crawlerStepTypes, Collections.unmodifiableMap(stepTypeToTask));
    }
}
