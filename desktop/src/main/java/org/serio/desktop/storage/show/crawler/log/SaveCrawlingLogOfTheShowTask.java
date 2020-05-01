package org.serio.desktop.storage.show.crawler.log;

import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.List;
import java.util.Properties;
import java.util.stream.Collectors;

public class SaveCrawlingLogOfTheShowTask implements StorageTask<Void> {
    private final String showId;
    private final List<CrawlLogEntry> log;
    private final Properties queries;

    public SaveCrawlingLogOfTheShowTask(String showId, List<CrawlLogEntry> log, Properties queries) {
        this.showId = showId;
        this.log = log;
        this.queries = queries;
    }

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
