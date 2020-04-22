package org.serio.core.showcrawlerlogstorage;

import java.util.Objects;
import java.util.Optional;

/**
 * A single crawl log entry.
 *
 * <p>Each crawl log entry can be made either by a crawler step or by some other part of the crawling process.
 * If an entry was made by a crawler step - it will have input and output information specified.</p>
 */
public class CrawlLogEntry {
    private final String value;
    private final String inputInformation;
    private final String outputInformation;

    /**
     * @see CrawlLogEntry#CrawlLogEntry(String, String, String)
     */
    public CrawlLogEntry(String value) {
        this(value, null, null);
    }

    /**
     * Construct a log entry.
     *
     * @param value actual body of the log entry. Can't be null.
     * @param inputInformation additional information about the input data of the corresponding crawl step
     * @param outputInformation additional information about the ouput data of the corresponding crawl step
     */
    public CrawlLogEntry(String value, String inputInformation, String outputInformation) {
        if (value == null) {
            throw new IllegalArgumentException("Crawl log entry value is not specified");
        }
        this.value = value;
        this.inputInformation = inputInformation;
        this.outputInformation = outputInformation;
    }

    /**
     * Check if the entry has more information attached to it than just a plain value.
     *
     * @return true if the entry has additional information attached to it
     */
    public boolean hasDetails() {
        return inputInformation != null || outputInformation != null;
    }

    /**
     * Get actual text value of the entry.
     *
     * @return text line of this log entry
     */
    public String getValue() {
        return value;
    }

    /**
     * Get additional information about input data, that was passed to the crawler step, that has made this entry.
     *
     * @return input data of the crawler step. Will be empty if the entry was not made by a crawler step.
     */
    public Optional<String> getInputInformation() {
        return Optional.ofNullable(inputInformation);
    }

    /**
     * Get additional information about output data, that was passed to the crawler step, that has made this entry.
     *
     * @return output data of the crawler step. Will be empty if the entry was not made by a crawler step
     */
    public Optional<String> getOutputInformation() {
        return Optional.ofNullable(outputInformation);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        CrawlLogEntry that = (CrawlLogEntry) o;
        return Objects.equals(value, that.value) &&
                Objects.equals(inputInformation, that.inputInformation) &&
                Objects.equals(outputInformation, that.outputInformation);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(value, inputInformation, outputInformation);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "CrawlLogEntry{" +
                "value='" + value + '\'' +
                ", inputInformation='" + inputInformation + '\'' +
                ", outputInformation='" + outputInformation + '\'' +
                '}';
    }
}
