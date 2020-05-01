package org.serio.core.applicationcontroller.model;

import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;

import java.util.Objects;
import java.util.Optional;

/**
 * Representation of {@link CrawlLogEntry} that contains information about it's positions in it's log.
 *
 * <p>Serves as an interface adapter between a {@link CrawlLogEntry} and the representation, that is
 * understandable by the user interface.</p>
 */
public class IndexedCrawlLogEntry {
    private final int id;
    private final CrawlLogEntry entry;

    /**
     * Construct an indexed crawl log entry based off of the specified one.
     *
     * @param id index of the entry in it's log
     * @param entry actual entry
     */
    public IndexedCrawlLogEntry(int id, CrawlLogEntry entry) {
        if (entry == null) {
            throw new IllegalArgumentException("Crawl log entry is not specified");
        }
        this.id = id;
        this.entry = entry;
    }

    /**
     * Get index of the entry in it's log.
     *
     * @return index of this entry
     */
    public int getId() {
        return id;
    }

    /**
     * @see CrawlLogEntry#getValue()
     */
    public String getDescription() {
        return entry.getValue();
    }

    /**
     * @see CrawlLogEntry#hasDetails()
     */
    public boolean getHasDetails() {
        return entry.hasDetails();
    }

    /**
     * @see CrawlLogEntry#getInputInformation()
     */
    public Optional<String> getInput() {
        return entry.getInputInformation();
    }

    /**
     * @see CrawlLogEntry#getOutputInformation()
     */
    public Optional<String> getOutput() {
        return entry.getOutputInformation();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        IndexedCrawlLogEntry that = (IndexedCrawlLogEntry) o;
        return id == that.id &&
                Objects.equals(entry, that.entry);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(id, entry);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "IndexedCrawlLogEntry{" +
                "id=" + id +
                ", entry=" + entry +
                '}';
    }
}
