package org.serio.core.showscrawler.tasks;

import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.core.showscrawler.CrawlingResult;
import org.serio.core.showscrawler.crawler.Crawler;
import org.serio.core.showscrawler.crawler.CrawlerStep;
import org.serio.core.showscrawler.crawler.CrawlerStepTypes;
import org.serio.core.showscrawler.crawler.UnknownCrawlerStepTypeException;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
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
    private final long logDetailsLength;
    private final CrawlerStepTypes crawlerStepTypes;
    private final Map<String, CrawlerStepTask> stepTypeToTask;

    /**
     * Construct a task, that will execute a crawler.
     *
     * @param crawler          crawler to execute
     * @param input            optional input data, that should be passed to the first crawler step of the specified crawler. Can
     * @param logDetailsLength maximum length of {@link CrawlLogEntry#getInputInformation()} and
     *                         {@link CrawlLogEntry#getOutputInformation()} of each {@link CrawlLogEntry} generated during the crawling
     * @param crawlerStepTypes collection of all possible {@link CrawlerStep} types. Will be used to enrich the
     *                         exception message in case the task will stumble upon a step with an unknown type.
     * @param stepTypeToTask   a map where each key is a crawler step type and each value is a corresponding
     *                         {@link CrawlerStepTask} to be executed upon such step
     */
    public CrawlerTask(Crawler crawler, List<String> input, long logDetailsLength, CrawlerStepTypes crawlerStepTypes,
                       Map<String, CrawlerStepTask> stepTypeToTask) {
        this.crawler = crawler;
        this.input = Collections.unmodifiableList(input != null ? input : new ArrayList<>());
        this.logDetailsLength = logDetailsLength;
        this.crawlerStepTypes = crawlerStepTypes;
        this.stepTypeToTask = stepTypeToTask;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public CrawlingResult call() throws Exception {
        try {
            List<String> data = input;
            List<CrawlLogEntry> log = new ArrayList<>();
            for (CrawlerStep step : crawler.getSteps()) {
                CrawlerStepTask task = stepTypeToTask.get(step.getType());
                if (task == null) {
                    throw new UnknownCrawlerStepTypeException(step.getType(), crawlerStepTypes.getAllTypes());
                }
                task = new LoggingTaskDecorator(task, log, logDetailsLength);
                data = task.execute(step, data);
            }
            return new CrawlingResult(data, log);
        } catch (Exception e) {
            throw new CrawlerTaskException(crawler, e);
        }
    }
}
