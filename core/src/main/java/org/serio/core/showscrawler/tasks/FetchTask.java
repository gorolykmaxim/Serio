package org.serio.core.showscrawler.tasks;

import org.serio.core.httpclient.HttpClient;
import org.serio.core.showscrawler.crawler.CrawlerStep;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.stream.Collectors;

/**
 * Executes a {@link CrawlerTask} with a "fetch" type.
 *
 * <p>Treats array of input strings from the previous steps as URLs and fetches their contents. Returns a corresponding
 * array of raw string response bodies.</p>
 */
public class FetchTask implements CrawlerStepTask {
    private final HttpClient httpClient;

    /**
     * Construct a task.
     *
     * @param httpClient http client to fetch link content with
     */
    public FetchTask(HttpClient httpClient) {
        this.httpClient = httpClient;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public List<String> execute(CrawlerStep step, List<String> input) {
        try {
            List<Future<String>> responseFutures = input
                    .stream()
                    .map(httpClient::fetchContentFromUrl)
                    .collect(Collectors.toList());
            List<String> responses = new ArrayList<>(responseFutures.size());
            for (Future<String> responseFuture : responseFutures) {
                responses.add(responseFuture.get());
            }
            return responses;
        } catch (InterruptedException | ExecutionException e) {
            throw new CrawlerStepTaskException(step, e);
        }
    }
}
