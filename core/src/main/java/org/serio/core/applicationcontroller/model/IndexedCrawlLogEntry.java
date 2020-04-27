package org.serio.core.applicationcontroller.model;

import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;

import java.util.Objects;
import java.util.Optional;

public class IndexedCrawlLogEntry {
    private final int id;
    private final CrawlLogEntry entry;

    public IndexedCrawlLogEntry(int id, CrawlLogEntry entry) {
        if (entry == null) {
            throw new IllegalArgumentException("Crawl log entry is not specified");
        }
        this.id = id;
        this.entry = entry;
    }

    public int getId() {
        return id;
    }

    public String getDescription() {
        return entry.getValue();
    }

    public boolean hasDetails() {
        return entry.hasDetails();
    }

    public Optional<String> getInput() {
        return entry.getInputInformation();
    }

    public Optional<String> getOutput() {
        return entry.getOutputInformation();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        IndexedCrawlLogEntry that = (IndexedCrawlLogEntry) o;
        return id == that.id &&
                Objects.equals(entry, that.entry);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, entry);
    }

    @Override
    public String toString() {
        return "IndexedCrawlLogEntry{" +
                "id=" + id +
                ", entry=" + entry +
                '}';
    }
}
