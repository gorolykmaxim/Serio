package org.serio.desktop.storage.show.crawler.log;

import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.List;
import java.util.Properties;
import java.util.stream.Collectors;

/**
 * Save the specified crawling log of the show with the specified ID.
 *
 * <p>This will override the previous crawling log.</p>
 */
public class SaveCrawlingLogOfTheShowTask implements StorageTask<Void> {
    private final String showId;
    private final List<CrawlLogEntry> log;
    private final Properties queries;

    /**
     * Construct a task.
     *
     * @param showId ID of the show, crawling log of which is being saved
     * @param log crawling log to save
     * @param queries {@link Properties}, that contains the SQL queries, used by this task
     */
    public SaveCrawlingLogOfTheShowTask(String showId, List<CrawlLogEntry> log, Properties queries) {
        this.showId = showId;
        this.log = log;
        this.queries = queries;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Void execute(JdbcTemplate template) {
        template.update(queries.getProperty("delete-last-crawling-log-of-the-show"), showId);
        List<Object[]> arguments = log.stream()
                .map(entry -> new Object[] {
                        showId,
                        entry.getValue(),
                        entry.getInputInformation().orElse(null),
                        entry.getOutputInformation().orElse(null)
                })
                .collect(Collectors.toList());
        template.batchUpdate(queries.getProperty("create-crawling-entry"), arguments);
        return null;
    }
}
