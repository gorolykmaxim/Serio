package org.serio.core.showscrawler.tasks;

import org.serio.core.httpclient.HttpClient;
import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.core.showscrawler.CrawlingResult;
import org.serio.core.showscrawler.crawler.*;
import org.serio.core.showscrawler.crawler.step.*;

import java.util.*;
import java.util.concurrent.Callable;

/**
 * A special kind of task, responsible for execution of an entire {@link Crawler}.
 *
 * <p>Goes through a list of steps of the specified crawler, processes each one of them sequentially, while
 * delegating each step execution to a corresponding implementation of the {@link CrawlerStepTask}.</p>
 *
 * <p>Generates and collects a crawling log from each crawler step for a debug purposes.</p>
 */
public class CrawlerTask implements Callable<CrawlingResult> {
    private final Crawler crawler;
    private final List<String> input;
    private final HttpClient httpClient;
    private final long logDetailsLength;

    /**
     * @see CrawlerTask#CrawlerTask(Crawler, HttpClient, long, List)
     */
    public CrawlerTask(Crawler crawler, HttpClient httpClient, long logDetailsLength) {
        this(crawler, httpClient, logDetailsLength, null);
    }

    /**
     * Construct a task, that will execute a crawler.
     *
     * @param crawler crawler to execute
     * @param httpClient http client, that can be used by crawler steps, that require it
     * @param logDetailsLength maximum length of {@link CrawlLogEntry#getInputInformation()} and
     * {@link CrawlLogEntry#getOutputInformation()} of each {@link CrawlLogEntry} generated during the crawling
     * @param input optional input data, that should be passed to the first crawler step of the specified crawler. Can
     *              be null
     */
    public CrawlerTask(Crawler crawler, HttpClient httpClient, long logDetailsLength, List<String> input) {
        this.crawler = crawler;
        this.httpClient = httpClient;
        this.logDetailsLength = logDetailsLength;
        this.input = Collections.unmodifiableList(input != null ? input : new ArrayList<>());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public CrawlingResult call() throws Exception {
        try {
            List<String> data = input;
            List<CrawlLogEntry> log = new ArrayList<>();
            for (CrawlerStep step: crawler.getSteps()) {
                CrawlerStepTask task;
                if (step instanceof ValueStep) {
                    task = new ValueTask((ValueStep) step);
                } else if (step instanceof FetchStep) {
                    task = new FetchTask((FetchStep) step, httpClient);
                } else if (step instanceof TransformStep) {
                    task = new TransformTask((TransformStep) step);
                } else if (step instanceof RegExpStep) {
                    task = new RegExpTask((RegExpStep) step);
                } else {
                    throw new IllegalArgumentException(String.format("%s does not support crawler steps with type '%s'", getClass().getName(), step.getType()));
                }
                task = new LoggingTaskDecorator(task, step, log, logDetailsLength);
                data = task.execute(data);
            }
            return new CrawlingResult(data, log);
        } catch (Exception e) {
            throw new CrawlerTaskException(crawler, e);
        }
    }
}
