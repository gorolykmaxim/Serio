package org.serio.core.showscrawler.tasks;

import org.serio.core.httpclient.HttpClient;
import org.serio.core.showscrawler.crawler.step.FetchStep;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.stream.Collectors;

/**
 * Executes a {@link FetchStep}.
 *
 * @see FetchStep
 */
public class FetchTask implements CrawlerStepTask {
    private final FetchStep step;
    private final HttpClient httpClient;

    /**
     * Construct a task.
     *
     * @param step crawler step to execute
     * @param httpClient http client to fetch link content with
     */
    public FetchTask(FetchStep step, HttpClient httpClient) {
        this.step = step;
        this.httpClient = httpClient;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public List<String> execute(List<String> input) {
        try {
            List<Future<String>> responseFutures = input
                    .stream()
                    .map(httpClient::fetchContentFromUrl)
                    .collect(Collectors.toList());
            List<String> responses = new ArrayList<>(responseFutures.size());
            for (Future<String> responseFuture: responseFutures) {
                responses.add(responseFuture.get());
            }
            return responses;
        } catch (InterruptedException | ExecutionException e) {
            throw new CrawlerStepTaskException(step, e);
        }
    }
}
